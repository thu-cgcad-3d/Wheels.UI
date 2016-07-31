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

#include <QtOpenGL>

#include "wheels/vision.hpp"

#include "shader.hpp"
#include "program.hpp"

namespace wheels {
// qt_opengl_manager
class qt_opengl_manager {
public:
  qt_opengl_manager();
  explicit qt_opengl_manager(QOpenGLContext *context);
  ~qt_opengl_manager();

public:
  void initialize_opengl() const;

  // create_shader
  std::unique_ptr<opengl::shader> create_shader(opengl::shader::type t,
                                                const char *src) const;
  // create_program
  std::unique_ptr<opengl::program>
  create_program(const std::vector<opengl::shader *> &shaders) const;

  // 

protected:
  opengl::glfunctions *glfun;
};
}