#pragma once

#include <QtOpenGL>
#include <qopenglfunctions_3_2_compatibility.h>
#include <qopenglfunctions_3_2_core.h>
#include <qopenglfunctions_3_3_core.h>
#include <qopenglfunctions_4_0_core.h>
#include <qopenglfunctions_4_1_core.h>
#include <qopenglfunctions_4_2_core.h>
#include <qopenglfunctions_4_3_core.h>

namespace wheels {
namespace opengl {
template <class T> struct data_type {};
template <> struct data_type<int8_t> {
  static constexpr GLenum value = GL_BYTE;
};
template <> struct data_type<uint8_t> {
  static constexpr GLenum value = GL_UNSIGNED_BYTE;
};
template <> struct data_type<int16_t> {
  static constexpr GLenum value = GL_SHORT;
};
template <> struct data_type<uint16_t> {
  static constexpr GLenum value = GL_UNSIGNED_SHORT;
};
template <> struct data_type<int32_t> {
  static constexpr GLenum value = GL_INT;
};
template <> struct data_type<uint32_t> {
  static constexpr GLenum value = GL_UNSIGNED_INT;
};
template <> struct data_type<float> {
  static constexpr GLenum value = GL_FLOAT;
};
template <> struct data_type<double> {
  static constexpr GLenum value = GL_DOUBLE;
};

enum class pixel_format : GLenum {
  color_index = GL_COLOR_INDEX,
  stencil_index = GL_STENCIL_INDEX,
  depth_component = GL_DEPTH_COMPONENT,
  red = GL_RED,
  green = GL_GREEN,
  blue = GL_BLUE,
  alpha = GL_ALPHA,
  rgb = GL_RGB,
  rgba = GL_RGBA,
  luminance = GL_LUMINANCE,
  luminance_alpha = GL_LUMINANCE_ALPHA,
};

enum class texture_type : GLenum {
  _1d = GL_TEXTURE_1D,
  _2d = GL_TEXTURE_2D,
  _3d = GL_TEXTURE_3D,
  _1d_array = GL_TEXTURE_1D_ARRAY,
  _2d_array = GL_TEXTURE_2D_ARRAY,
  cubemap = GL_TEXTURE_CUBE_MAP,
  cubemap_array = GL_TEXTURE_CUBE_MAP_ARRAY,
};
enum class texture_warping : GLenum {
  repeat = GL_REPEAT,
  mirrored_repeat = GL_MIRRORED_REPEAT,
  clamp_to_edge = GL_CLAMP_TO_EDGE,
  clamp_to_border = GL_CLAMP_TO_BORDER,
};
enum class texture_filtering : GLenum {
  nearest = GL_NEAREST,
  linear = GL_LINEAR,
  nearest_mipmap_nearest = GL_NEAREST_MIPMAP_NEAREST,
  linear_mipmap_nearest = GL_LINEAR_MIPMAP_NEAREST,
  nearest_mipmap_linear = GL_NEAREST_MIPMAP_LINEAR,
  linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR,
};

enum class buffer_type : GLenum {
  // The buffer will be used as a source for vertex data, but the connection
  // is only made when glVertexAttribPointer is called. The pointer field of
  // this function is taken as a byte offset from the beginning of whatever
  // buffer is currently bound to this target.
  array = GL_ARRAY_BUFFER,
  // All rendering functions of the form gl*Draw*Elements* will use the
  // pointer field as a byte offset from the beginning of the buffer object
  // bound to this target. The indices used for indexed rendering will be
  // taken from the buffer object. Note that this binding target is part of a
  // Vertex Array Objects state, so a VAO must be bound before binding a
  // buffer here.
  element_array = GL_ELEMENT_ARRAY_BUFFER,
  // These have no particular semantics. Because they have no actual meaning,
  // they are useful targets for copying buffer object data with
  // glCopyBufferSubData. You do not have to use these targets when copying,
  // but by using them, you avoid disturbing buffer targets that have actual
  // semantics.
  copy_read = GL_COPY_READ_BUFFER,
  copy_write = GL_COPY_WRITE_BUFFER,
  // These are for performing asynchronous pixel transfer operations. If a
  // buffer is bound to GL_PIXEL_UNPACK_BUFFER, glTexImage*, glTexSubImage*,
  // glCompressedTexImage*, and glCompressedTexSubImage* are all affected.
  // These functions will read their data from the bound buffer object instead
  // of where a client pointer points. Similarly, if a buffer is bound to
  // GL_PIXEL_PACK_BUFFER, glGetTexImage, and glReadPixels will store their
  // data to the bound buffer object instead of where a client pointer points.
  pixel_unpack = GL_PIXEL_UNPACK_BUFFER,
  pixel_pack = GL_PIXEL_PACK_BUFFER,
  // These are for performing direct writes from asynchronous queries to
  // buffer object memory. If a buffer is bound to GL_QUERY_BUFFER then all
  // glGetQueryObject[ui64v] function calls will write the result to an offset
  // into the bound buffer object.
  query = GL_QUERY_BUFFER,
  // This target has no special semantics, but if you intend to use a buffer
  // object for Buffer Textures, it is a good idea to bind it here when you
  // first create it.
  texture = GL_TEXTURE_BUFFER,
  // An indexed buffer binding for buffers used in Transform Feedback
  // operations.
  transform_feedback = GL_TRANSFORM_FEEDBACK_BUFFER,
  // An indexed buffer binding for buffers used as storage for uniform blocks.
  uniform = GL_UNIFORM_BUFFER,
  // The buffer bound to this target will be used as the source for the
  // indirect data when performing indirect rendering. This is only available
  // in core OpenGL 4.0 or with ARB_draw_indirec
  draw_indirect = GL_DRAW_INDIRECT_BUFFER,
  // An indexed buffer binding for buffers used as storage for atomic
  // counters. This requires OpenGL 4.2 or ARB_shader_atomic_counters
  atomic_counter = GL_ATOMIC_COUNTER_BUFFER,
  // The buffer bound to this target will be used as the source for indirect
  // compute dispatch operations, via glDispatchComputeIndirect. This
  // requires OpenGL 4.3 or ARB_compute_shader
  dispatch_indirect = GL_DISPATCH_INDIRECT_BUFFER,
  // An indexed buffer binding for buffers used as storage for shader storage
  // blocks. This requires OpenGL 4.3 or ARB_shader_storage_buffer_object
  shader_storage = GL_SHADER_STORAGE_BUFFER,
};

enum class buffer_usage : GLenum {
  stream_draw = GL_STREAM_DRAW,
  stream_read = GL_STREAM_READ,
  stream_copy = GL_STREAM_COPY,
  static_draw = GL_STATIC_DRAW,
  static_read = GL_STATIC_READ,
  static_copy = GL_STATIC_COPY,
  dynamic_draw = GL_DYNAMIC_DRAW,
  dynamic_read = GL_DYNAMIC_READ,
  dynamic_copy = GL_DYNAMIC_COPY,
};

enum class shader_type : GLenum {
  vertex = GL_VERTEX_SHADER,
  tess_control = GL_TESS_CONTROL_SHADER,
  tess_evaluation = GL_TESS_EVALUATION_SHADER,
  geometry = GL_GEOMETRY_SHADER,
  fragment = GL_FRAGMENT_SHADER,
  compute = GL_COMPUTE_SHADER
};

enum class draw_mode : GLenum {
  points = GL_POINTS,
  lines = GL_LINES,
  line_loop = GL_LINE_LOOP,
  line_strip = GL_LINE_STRIP,
  triangles = GL_TRIANGLES,
  triangle_strip = GL_TRIANGLE_STRIP,
  triangle_fan = GL_TRIANGLE_FAN,
  quads = GL_QUADS,
  quad_strip = GL_QUAD_STRIP,
  polygon = GL_POLYGON,
};

enum class vertex_attribute { position, normal, tex_coord };
enum class texture_attribute {
  normal,
  ambient,
  diffuse,
  specular,
  alpha,
  std_tex_num,
  shadow_map_0 = std_tex_num
};

using glcontext = QOpenGLContext;
using glfunctions = QOpenGLFunctions_4_3_Core;
}
}