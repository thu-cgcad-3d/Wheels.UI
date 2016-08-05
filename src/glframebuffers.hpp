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
class framebuffers : public object {
public:
  explicit framebuffers(glfunctions *fun, size_t n);
  explicit framebuffers(glfunctions *fun, GLuint *fbs, size_t n);
  virtual ~framebuffers();

  framebuffers(const framebuffers &) = delete;
  framebuffers(framebuffers && fb);
  framebuffers & operator=(const framebuffers &) = delete;
  framebuffers & operator=(framebuffers && fb);

  void swap(framebuffers & fb);
  size_t size() const { return _size; }

  framebuffers operator[](size_t i) const;

private:
  size_t _size;
  GLuint *_fbs;
  bool _own_res;
};
}
}