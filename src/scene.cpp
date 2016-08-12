#include "utility.hpp"

#include "scene.hpp"

#include "wheels/tensor.hpp"

namespace wheels {
scene::scene()
    : _glfun(nullptr), _camera(vec3f(5, 5, 4), vec3f(0, 0, 0), vec3f(0, 0, 1),
                               500, 500, 500, vec2f(250, 250), 0.001f, 1e4f),
      _background(0.1f, 0.1f, 0.1f) {}
scene::~scene() {
  if (!_glfun) {
    return;
  }
  for (auto &gd : _name2geo) {
    _glfun->glDeleteBuffers(2, gd.second.buffers);
    _glfun->glDeleteVertexArrays(1, &gd.second.vao);
  }
  for (auto &md : _name2mat) {
    _glfun->glDeleteProgram(md.second.first_pass.program);
    _glfun->glDeleteProgram(md.second.second_pass.program);
    for (GLuint tex : md.second.tex) {
      if (tex != -1) {
        _glfun->glDeleteTextures(1, &tex);
      }
    }
  }
  for (auto &ld : _light_data_table) {
    _glfun->glDeleteFramebuffers(1, &ld.fbo_shadow);
    _glfun->glDeleteTextures(1, &ld.tex_depth_map);
  }
}

void scene::setup(opengl::glfunctions *glf) { _glfun = glf; }

namespace detail {
template <class E, class IndexT, size_t N>
void _convert_mesh_to_vao(opengl::glfunctions *glfun,
                          const render_mesh<E, IndexT, N> &mesh, GLuint &vao,
                          GLuint *buffers, GLenum &draw_mode, GLsizei &count,
                          GLenum &index_type) {
  glfun->glGenBuffers(2, buffers);

  glfun->glGenVertexArrays(1, &vao);
  glfun->glBindVertexArray(vao);
  glfun->glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
  glfun->glBufferData(GL_ARRAY_BUFFER, mesh.nbytes_of_verts(), mesh.verts_ptr(),
                      GL_STATIC_DRAW);
  using vertex = typename render_mesh<E, IndexT, N>::vertex;
  static_assert(sizeof(vertex) == 8 * sizeof(float), "");

  glfun->glEnableVertexAttribArray(
      underlying(opengl::vertex_attribute::position));
  glfun->glVertexAttribPointer(underlying(opengl::vertex_attribute::position),
                               3, opengl::data_type<E>::value, GL_FALSE,
                               sizeof(vertex),
                               (void *)offsetof(vertex, position));

  glfun->glEnableVertexAttribArray(
      underlying(opengl::vertex_attribute::normal));
  glfun->glVertexAttribPointer(underlying(opengl::vertex_attribute::normal), 3,
                               opengl::data_type<E>::value, GL_FALSE,
                               sizeof(vertex),
                               (void *)offsetof(vertex, normal));

  glfun->glEnableVertexAttribArray(
      underlying(opengl::vertex_attribute::tex_coord));
  glfun->glVertexAttribPointer(underlying(opengl::vertex_attribute::tex_coord),
                               2, opengl::data_type<E>::value, GL_FALSE,
                               sizeof(vertex),
                               (void *)offsetof(vertex, tex_coord));

  glfun->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
  glfun->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.nbytes_of_indices(),
                      mesh.indices_ptr(), GL_STATIC_DRAW);
  glfun->glBindVertexArray(0);

  switch (N) {
  case 1:
    draw_mode = GL_POINTS;
    break;
  case 2:
    draw_mode = GL_LINES;
    break;
  case 3:
    draw_mode = GL_TRIANGLES;
    break;
  default:
    throw std::runtime_error("unsupported dimension!");
    static_assert(N == 1 || N == 2 || N == 3, "");
  }

  count = (GLsizei)mesh.indices().numel() * N;
  index_type = opengl::data_type<IndexT>::value;
  assert(index_type == GL_UNSIGNED_BYTE || index_type == GL_UNSIGNED_SHORT ||
         index_type == GL_UNSIGNED_INT);
}
}

void scene::add_geometry(const std::string &name,
                         const render_mesh<float, uint32_t, 3> &mesh) {
  auto &gd = _name2geo[name];
  gd.mesh = mesh;
  detail::_convert_mesh_to_vao(_glfun, mesh, gd.vao, gd.buffers, gd.draw_mode,
                               gd.count, gd.index_type);
}

void scene::add_geometry(const std::string &name,
                         render_mesh<float, uint32_t, 3> &&mesh) {
  auto &gd = _name2geo[name];
  detail::_convert_mesh_to_vao(_glfun, mesh, gd.vao, gd.buffers, gd.draw_mode,
                               gd.count, gd.index_type);
  gd.mesh = std::move(mesh);
}

namespace detail {
GLuint _compile_shader(opengl::glfunctions *glfun, const char *src,
                       GLenum type) {
  GLuint s = glfun->glCreateShader(type);
  GLint len = (GLint)strlen(src);
  glfun->glShaderSource(s, 1, &src, &len);
  glfun->glCompileShader(s);
  GLint is_compiled = 0;
  glfun->glGetShaderiv(s, GL_COMPILE_STATUS, &is_compiled);
  if (!is_compiled) {
    GLint log_size = 0;
    glfun->glGetShaderiv(s, GL_INFO_LOG_LENGTH, &log_size);
    std::string log(log_size + 1, '\0');
    glfun->glGetShaderInfoLog(s, log_size, &log_size, &log[0]);
    std::cerr << log << std::endl;
    glfun->glDeleteShader(s);
    return 0;
  }
  return s;
}
GLuint _compile_shader_program(opengl::glfunctions *glfun, const char *vs,
                               const char *gs, const char *fs) {
  GLuint program = glfun->glCreateProgram();
  GLuint vshader, gshader, fshader;
  if (vs) {
    vshader = _compile_shader(glfun, vs, GL_VERTEX_SHADER);
    glfun->glAttachShader(program, vshader);
  }
  if (fs) {
    fshader = _compile_shader(glfun, fs, GL_FRAGMENT_SHADER);
    glfun->glAttachShader(program, fshader);
  }
  if (gs) {
    gshader = _compile_shader(glfun, gs, GL_GEOMETRY_SHADER);
    glfun->glAttachShader(program, gshader);
  }
  glfun->glLinkProgram(program);

  GLint is_linked = 0;
  glfun->glGetProgramiv(program, GL_LINK_STATUS, &is_linked);
  if (!is_linked) {
    GLint log_size = 0;
    glfun->glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_size);
    std::string log(log_size + 1, '\0');
    glfun->glGetProgramInfoLog(program, log_size, &log_size, &log[0]);
    std::cerr << log << std::endl;
    glfun->glDeleteProgram(program);
    return 0;
  }
  return program;
}

// _convert_image_to_texture
template <class E, size_t N>
GLuint _convert_image_to_texture(opengl::glfunctions *glfun,
                                 const image_<E, N> &im) {
  GLuint texture = 0;
  glfun->glGenTextures(1, &texture);
  glfun->glBindTexture(GL_TEXTURE_2D, texture);

  GLenum pixel_format;
  switch (N) {
  case 1:
    pixel_format = GL_ALPHA;
    break;
  case 3:
    pixel_format = GL_RGB;
    break;
  case 4:
    pixel_format = GL_RGBA;
    break;
  default:
    throw std::runtime_error("unsupported number of channels!");
    static_assert(N == 1 || N == 3 || N == 4, "");
  }
  glfun->glTexImage2D(GL_TEXTURE_2D, 0, pixel_format, im.width(), im.height(),
                      0, pixel_format, opengl::data_type<E>::value, im.ptr());
  glfun->glGenerateMipmap(GL_TEXTURE_2D);

  glfun->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glfun->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  float borderColor[] = {0.0f, 0.0f, 0.0f, 1.0f};
  glfun->glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
  glfun->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glfun->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glfun->glBindTexture(GL_TEXTURE_2D, 0);

  return texture;
}
}

static constexpr int SHADOW_WIDTH = 1024;
static constexpr int SHADOW_HEIGHT = 1024;

void scene::add_material(const std::string &name, const material &mat) {
  material_data mat_data;

  // the shading program
  const char *vshader_shading_src = R"SHADER(
      #version 330 core
      layout (location = 0) in vec3 position;
      //layout (location = 2) in vec2 tex_coord;      

      uniform mat4 model_matrix;

      void main(){
        gl_Position = model_matrix * vec4(position, 1.0);
      }  
    )SHADER";
  const char *gshader_shading_src = R"SHADER(
      #version 330 core
      layout (triangles) in;
      layout (triangle_strip, max_vertices=18) out;

      uniform mat4 shadow_matrices[6];      
      out vec4 FragPos; // FragPos from GS (output per emitvertex)

      void main(){
        for(int face = 0; face < 6; ++face){
          gl_Layer = face; // built-in variable that specifies to which face we render.
          for(int i = 0; i < 3; ++i) {// for each triangle's vertices
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadow_matrices[face] * FragPos;
            EmitVertex();
          }    
          EndPrimitive();
        }
      }
    )SHADER";
  const char *fshader_shading_src = R"SHADER(
      #version 330 core
      in vec4 FragPos;

      uniform vec3 light_position;
      uniform float far_plane;

      bool IsSolid(vec3 frag_pos, vec2 tex_coord){
        return true;
      }

      void main(){
        // get distance between fragment and light source
        float light_distance = length(FragPos.xyz - light_position);
    
        // map to [0;1] range by dividing by far_plane
        light_distance = light_distance / far_plane;
    
        // Write this as modified depth
        gl_FragDepth = light_distance;
      } 
    )SHADER";
  GLuint program_shading = detail::_compile_shader_program(
      _glfun, vshader_shading_src, gshader_shading_src, fshader_shading_src);
  assert(error());

  mat_data.first_pass.program = program_shading;
  mat_data.first_pass.uniform_model_matrix =
      _glfun->glGetUniformLocation(mat_data.first_pass.program, "model_matrix");
  mat_data.first_pass.uniform_shadow_matrices = _glfun->glGetUniformLocation(
      mat_data.first_pass.program, "shadow_matrices");
  mat_data.first_pass.uniform_light_position = _glfun->glGetUniformLocation(
      mat_data.first_pass.program, "light_position");
  mat_data.first_pass.uniform_far_plane =
      _glfun->glGetUniformLocation(mat_data.first_pass.program, "far_plane");

  // the rendering program
  constexpr char *vshader_rendering_src = R"SHADER(
      #version 330 core
      uniform mat4 view_matrix;
      uniform mat4 proj_matrix;
      uniform mat4 model_matrix;
    
      layout(location = 0) in vec3 position;
      layout(location = 1) in vec3 normal;
      layout(location = 2) in vec2 tex_coord;

      out VS_OUT {
        vec3 frag_pos;
        vec3 normal;
        vec2 tex_coord;
      } vs_out;

      void main(){
        gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0);
        vs_out.frag_pos = vec3(model_matrix * vec4(position, 1.0f));
        vs_out.normal = transpose(inverse(mat3(model_matrix))) * normal;
        vs_out.tex_coord = tex_coord;
      }
    )SHADER";
  const std::string fshader_rendering_src = R"SHADER(
      #version 330 core
      #define MAX_NLIGHTS 16
      out vec4 FragColor;
      in VS_OUT {
        vec3 frag_pos;
        vec3 normal;
        vec2 tex_coord;
      } fs_in;

      struct optional_sampler2D {
        bool exists;
        sampler2D tex;
      };
      uniform optional_sampler2D tex_normal;
      uniform optional_sampler2D tex_ambient;
      uniform optional_sampler2D tex_diffuse;
      uniform optional_sampler2D tex_specular;
      uniform optional_sampler2D tex_alpha;      
    
      uniform uint nlights;
      uniform samplerCube depth_maps[MAX_NLIGHTS];
      uniform vec3 light_positions[MAX_NLIGHTS];
      uniform vec3 light_colors[MAX_NLIGHTS];
      uniform float far_planes[MAX_NLIGHTS];

      uniform vec3 eye;

      float ComputeShadow(int light_id){
        vec3 light_dir = normalize(light_positions[light_id] - fs_in.frag_pos);
        float bias = max(0.05 * (1.0 - dot(normalize(fs_in.normal), light_dir)), 0.005);
        vec3 frag_to_light = fs_in.frag_pos - light_positions[light_id]; 
        float closest_depth = texture(depth_maps[light_id], frag_to_light).r;
        closest_depth *= far_planes[light_id];  
        float current_depth = length(frag_to_light);  
        float shadow = current_depth -  bias > closest_depth ? 1.0 : 0.0; 
        return shadow;
      }
            
      vec4 ComputeColor(int valid_lights_num, 
        vec3 frag_pos, vec3 normal, vec2 tex_coord);

      void main(){
        FragColor = ComputeColor(min(int(nlights), MAX_NLIGHTS), 
          fs_in.frag_pos, normalize(fs_in.normal), fs_in.tex_coord);
      }    
    )SHADER";
  std::string combined_fshader_rendering_src =
      fshader_rendering_src + mat.compute_color_function;
  GLuint program_rendering =
      detail::_compile_shader_program(_glfun, vshader_rendering_src, nullptr,
                                      combined_fshader_rendering_src.c_str());
  assert(error());

  mat_data.second_pass.program = program_rendering;
  // model & camera related uniforms
  mat_data.second_pass.uniform_view_matrix =
      _glfun->glGetUniformLocation(mat_data.second_pass.program, "view_matrix");
  mat_data.second_pass.uniform_proj_matrix =
      _glfun->glGetUniformLocation(mat_data.second_pass.program, "proj_matrix");
  mat_data.second_pass.uniform_model_matrix = _glfun->glGetUniformLocation(
      mat_data.second_pass.program, "model_matrix");
  mat_data.second_pass.uniform_eye =
      _glfun->glGetUniformLocation(mat_data.second_pass.program, "eye");

  // light related uniforms
  mat_data.second_pass.uniform_depth_maps =
      _glfun->glGetUniformLocation(mat_data.second_pass.program, "depth_maps");
  mat_data.second_pass.uniform_light_positions = _glfun->glGetUniformLocation(
      mat_data.second_pass.program, "light_positions");
  mat_data.second_pass.uniform_light_colors = _glfun->glGetUniformLocation(
      mat_data.second_pass.program, "light_colors");
  mat_data.second_pass.uniform_far_planes =
      _glfun->glGetUniformLocation(mat_data.second_pass.program, "far_planes");
  mat_data.second_pass.uniform_nlights =
      _glfun->glGetUniformLocation(mat_data.second_pass.program, "nlights");

  // tex related uniforms
  const std::string std_texture_attribute_names[] = {
      "normal", "ambient", "diffuse", "specular", "alpha",
  };
  _glfun->glUseProgram(mat_data.second_pass.program);
  for_each(make_const_sequence(
               const_int<underlying(opengl::texture_attribute::std_tex_num)>()),
           [&](auto id) {
             constexpr int i = decltype(id)::value;
             constexpr const_ints<opengl::texture_attribute,
                                  opengl::texture_attribute(i)>
                 attribute;
             
             const std::string exists_name =
                 "tex_" + std_texture_attribute_names[i] + ".exists";
             const std::string tex_name =
                 "tex_" + std_texture_attribute_names[i] + ".tex";
             GLint uniform_exists = _glfun->glGetUniformLocation(
                 mat_data.second_pass.program, exists_name.c_str());
             GLint uniform_tex = _glfun->glGetUniformLocation(
                 mat_data.second_pass.program, tex_name.c_str());

             mat_data.tex[i] = (GLuint)-1;
             if (uniform_exists != -1 &&
                 uniform_tex != -1) { // the uniform is active
               bool has_tex = mat.texture(attribute) != nullptr;
               _glfun->glUniform1i(uniform_exists, has_tex);
               assert(error());
               _glfun->glUniform1i(uniform_tex, i);
               assert(error());
               if (has_tex) {
                 mat_data.tex[i] = detail::_convert_image_to_texture(
                     _glfun, *mat.texture(attribute));
                 assert(error());
               }
             }
             assert(error());
           });
  _glfun->glUseProgram(0);
  _name2mat[name] = mat_data;
}

static constexpr float FAR_PLANE = 50.0f;
void scene::add_light(const point_light<float> &l) {
  // gen the frame buffer
  GLuint fbo;
  _glfun->glGenFramebuffers(1, &fbo);

  // gen the cube map
  GLuint shadow_map;
  _glfun->glGenTextures(1, &shadow_map);
  _glfun->glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map);
  for (GLuint i = 0; i < 6; i++) {
    _glfun->glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                         GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0,
                         GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    assert(error());
  }
  _glfun->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
                          GL_NEAREST);
  _glfun->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
                          GL_NEAREST);
  _glfun->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                          GL_CLAMP_TO_EDGE);
  _glfun->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                          GL_CLAMP_TO_EDGE);
  _glfun->glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                          GL_CLAMP_TO_EDGE);
  assert(error());

  _glfun->glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  _glfun->glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_map,
                               0);
  _glfun->glDrawBuffer(GL_NONE);
  _glfun->glReadBuffer(GL_NONE);
  assert(error());
  {
    auto status = _glfun->glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch (status) {
    case GL_FRAMEBUFFER_UNDEFINED:
      std::cerr << "GL_FRAMEBUFFER_UNDEFINED" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
      std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
      std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
      std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER" << std::endl;
      break;
    case GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE:
      std::cerr << "GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE" << std::endl;
      break;
    case GL_FRAMEBUFFER_UNSUPPORTED:
      std::cerr << "GL_FRAMEBUFFER_UNSUPPORTED" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
      std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE" << std::endl;
      break;
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
      std::cerr << "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS" << std::endl;
      break;
    default:
      std::cout << "complete framebuffer!" << std::endl;
    }
    assert(status == GL_FRAMEBUFFER_COMPLETE);
  }

  light_data ld;
  ld.fbo_shadow = fbo;
  ld.tex_depth_map = shadow_map;

  int light_id = (int)_light_data_table.size();
  assert(all_same(light_id, _light_data_table.size(),
                  _depth_map_uniform_values.size(), _light_positions.size(),
                  _light_colors.size(), _far_planes.size(),
                  _light_shadow_matrices.size()));

  _glfun->glActiveTexture(GL_TEXTURE0 +
                          underlying(opengl::texture_attribute::shadow_map_0) +
                          light_id);
  _glfun->glBindTexture(GL_TEXTURE_CUBE_MAP, shadow_map);
  _glfun->glActiveTexture(GL_TEXTURE0);

  _depth_map_uniform_values.push_back(
      underlying(opengl::texture_attribute::shadow_map_0) + light_id);

  _light_positions.push_back(l.position);
  _light_colors.push_back(l.color);
  _far_planes.push_back(FAR_PLANE);

  // compute shadow matrices
  const mat4f shadow_projection_matrix = perspective<float>(
      numeric_<float>::PI / 2.0f, (float)SHADOW_WIDTH / (float)(SHADOW_HEIGHT),
      1.0f, FAR_PLANE);
  _light_shadow_matrices.push_back(vec_<mat4f, 6>(
      shadow_projection_matrix *
          look_at(l.position, l.position + vec3f(1, 0, 0), vec3f(0, -1, 0)),
      shadow_projection_matrix *
          look_at(l.position, l.position + vec3f(-1, 0, 0), vec3f(0, -1, 0)),
      shadow_projection_matrix *
          look_at(l.position, l.position + vec3f(0, 1, 0), vec3f(0, 0, 1)),
      shadow_projection_matrix *
          look_at(l.position, l.position + vec3f(0, -1, 0), vec3f(0, 0, -1)),
      shadow_projection_matrix *
          look_at(l.position, l.position + vec3f(0, 0, 1), vec3f(0, -1, 0)),
      shadow_projection_matrix *
          look_at(l.position, l.position + vec3f(0, 0, -1), vec3f(0, -1, 0))));

  assert(error());

  _light_data_table.push_back(ld);
}

box<vec3f> scene::get_bounding_box() const {
  box<vec3f> bb;
  for (auto &gd : _name2geo) {
    bb |= bounding_box(gd.second.mesh);
  }
  return bb;
}

void scene::init(GLuint default_fbo) {}

void scene::render(GLuint default_fbo) const {
  _glfun->glEnable(GL_DEPTH_TEST);
  _glfun->glEnable(GL_CULL_FACE);
  _glfun->glEnable(GL_MULTISAMPLE);
  _glfun->glClearColor(_background.r(), _background.g(), _background.b(), 1.0f);

  const size_t nlights = _light_data_table.size();

  // first pass
  _glfun->glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
  for (int light_id = 0; light_id < nlights; light_id++) {
    auto &light = _light_data_table[light_id];

    _glfun->glBindFramebuffer(GL_FRAMEBUFFER, light.fbo_shadow);
    _glfun->glBindTexture(GL_TEXTURE_CUBE_MAP, light.tex_depth_map);
    _glfun->glClear(GL_DEPTH_BUFFER_BIT);
    assert(error());

    for (auto &o : _name2obj) {
      auto &obj = o.second;
      auto &geo = _name2geo.at(obj.geo_name);
      auto &mat = _name2mat.at(obj.mat_name);
      _glfun->glUseProgram(mat.first_pass.program);
      _glfun->glUniformMatrix4fv(mat.first_pass.uniform_model_matrix, 1,
                                 GL_TRUE, o.second.model_matrix.ptr());
      _glfun->glUniformMatrix4fv(mat.first_pass.uniform_shadow_matrices, 6,
                                 GL_TRUE,
                                 _light_shadow_matrices[light_id][0].ptr());
      _glfun->glUniform1f(mat.first_pass.uniform_far_plane,
                          _far_planes[light_id]);
      _glfun->glUniform3fv(mat.first_pass.uniform_light_position, 1,
                           _light_positions[light_id].ptr());
      assert(error());
      _glfun->glBindVertexArray(geo.vao);
      _glfun->glDrawElements(geo.draw_mode, geo.count, geo.index_type, nullptr);
    }
  }

  // second pass
  _glfun->glBindFramebuffer(GL_FRAMEBUFFER, default_fbo);
  _glfun->glEnable(GL_MULTISAMPLE);
  _glfun->glViewport(0, 0, _camera.width, _camera.height);
  _glfun->glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  assert(error());

  for (auto &o : _name2obj) {
    auto &obj = o.second;
    auto &geo = _name2geo.at(obj.geo_name);
    auto &mat = _name2mat.at(obj.mat_name);
    _glfun->glUseProgram(mat.second_pass.program);
    _glfun->glUniformMatrix4fv(mat.second_pass.uniform_view_matrix, 1, GL_TRUE,
                               _camera.view_matrix().ptr());
    _glfun->glUniformMatrix4fv(mat.second_pass.uniform_proj_matrix, 1, GL_TRUE,
                               _camera.projection_matrix().ptr());
    _glfun->glUniformMatrix4fv(mat.second_pass.uniform_model_matrix, 1, GL_TRUE,
                               o.second.model_matrix.ptr());
    _glfun->glUniform3fv(mat.second_pass.uniform_eye, 1, _camera.eye.ptr());

    // light related uniforms
    _glfun->glUniform1ui(mat.second_pass.uniform_nlights, (GLuint)nlights);
    _glfun->glUniform1fv(mat.second_pass.uniform_far_planes, (GLuint)nlights,
                         _far_planes.data());
    _glfun->glUniform3fv(mat.second_pass.uniform_light_positions,
                         (GLuint)nlights, _light_positions[0].ptr());
    _glfun->glUniform3fv(mat.second_pass.uniform_light_colors, (GLuint)nlights,
                         _light_colors[0].ptr());
    _glfun->glUniform1iv(mat.second_pass.uniform_depth_maps, (GLuint)nlights,
                         _depth_map_uniform_values.data());

    // texture related uniforms
    for (int i = 0; i < underlying(opengl::texture_attribute::std_tex_num);
         i++) {
      if (mat.tex[i] != -1) {
        _glfun->glActiveTexture(GL_TEXTURE0 + i);
        _glfun->glBindTexture(GL_TEXTURE_2D, mat.tex[i]);
        _glfun->glActiveTexture(GL_TEXTURE0);
      }
    }
    assert(error());
    _glfun->glBindVertexArray(geo.vao);
    _glfun->glDrawElements(geo.draw_mode, geo.count, geo.index_type, nullptr);
    assert(error());
  }
}

bool scene::error() const {
  switch (_glfun->glGetError()) {
  case GL_INVALID_ENUM:
    std::cerr << "GL_INVALID_ENUM" << std::endl;
    return false;
  case GL_INVALID_VALUE:
    std::cerr << "GL_INVALID_VALUE" << std::endl;
    return false;
  case GL_INVALID_OPERATION:
    std::cerr << "GL_INVALID_OPERATION" << std::endl;
    return false;
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
    return false;
  case GL_OUT_OF_MEMORY:
    std::cerr << "GL_OUT_OF_MEMORY" << std::endl;
    return false;
  case GL_STACK_UNDERFLOW:
    std::cerr << "GL_STACK_UNDERFLOW" << std::endl;
    return false;
  case GL_STACK_OVERFLOW:
    std::cerr << "GL_STACK_OVERFLOW" << std::endl;
    return false;
  default:
    return true;
  }
}
}