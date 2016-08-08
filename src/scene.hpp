/* * *
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 Hao Yang (yangh2007@gmail.com)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * * */

#pragma once

#include <unordered_map>

#include "discretize.hpp"
#include "light.hpp"
#include "material.hpp"

namespace wheels {
namespace qt {
class SceneWidget;
}
class scene {
public:
  struct geometry_data {
    GLuint vao;
    GLuint buffers[2];
    GLenum draw_mode;
    GLsizei count;
    GLenum index_type;
  };
  struct material_data {
    struct {
      GLuint program;
      GLint uniform_model_matrix;
      GLint uniform_shadow_matrices;
      GLint uniform_light_position;
      GLint uniform_far_plane;
    } first_pass;
    struct {
      GLuint program;
      GLint uniform_view_matrix, uniform_proj_matrix, uniform_model_matrix;
      GLint uniform_tex_diffuse;
      GLint uniform_depth_map;
      GLint uniform_eye, uniform_light_position;
      GLint uniform_far_plane;
    } second_pass;
    GLuint tex[underlying(opengl::texture_attribute::std_tex_num)];
  };
  struct light_data {
    vec3f position;
    mat4f shadow_matrices[6];
    GLuint fbo_shadow;
    GLuint tex_shadow;
  };
  struct object_data {
    std::string geo_name;
    std::string mat_name;
    mat4f model_matrix;
  };

private:
  explicit scene();
  virtual ~scene();

  void setup(opengl::glfunctions *glf);
  opengl::glfunctions *glfun() const { return _glfun; }

public:
  void add(const std::string &name,
           const polygonal_mesh<float, uint32_t, 3> &mesh);
  void add(const std::string &name,
           const polygonal_mesh<float, uint32_t, 2> &mesh);

  void add(const std::string &name, const material &mat);

  void add(const std::string &name, const point_light<float> &l);

  void add_object(const std::string &name, const std::string &geo_name,
                  const std::string &mat_name);

  void set_camera(const perspective_camera<float> &c) { _camera = c; }
  const auto &camera() const { return _camera; }
  auto &camera() { return _camera; }

private:
  void render(GLuint default_fbo) const;
  bool error() const;

private:
  friend class qt::SceneWidget;
  opengl::glfunctions *_glfun;
  std::unordered_map<std::string, geometry_data> _name2geo;
  std::unordered_map<std::string, material_data> _name2mat;
  std::unordered_map<std::string, object_data> _name2obj;
  std::unordered_map<std::string, light_data> _name2light;
  perspective_camera<float> _camera;
};
}