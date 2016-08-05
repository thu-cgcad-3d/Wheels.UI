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

#include "globject.hpp"

namespace wheels {
namespace opengl {
// vertex_arrays
class vertex_arrays : public object {
public:
  enum class vertex_attribute {
    position,
    normal,
    tex_coord,
    color,
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

public:
  explicit vertex_arrays(glfunctions *fun, size_t n);
  explicit vertex_arrays(glfunctions *fun, GLuint *vaos, size_t n);
  
  virtual ~vertex_arrays();

  vertex_arrays(const vertex_arrays &) = delete;
  vertex_arrays(vertex_arrays &&fb);
  vertex_arrays &operator=(const vertex_arrays &) = delete;
  vertex_arrays &operator=(vertex_arrays &&fb);

  void swap(vertex_arrays &fb);
  size_t size() const { return _size; }

  vertex_arrays operator[](size_t i) const;

  //void draw() const;

private:
  size_t _size;
  GLuint *_vaos;
  bool _own_res;
};
}
}