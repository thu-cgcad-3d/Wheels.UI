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
class textures : public object {
public:
  enum class type : GLenum {
    texture1d = GL_TEXTURE_1D,
    texture2d = GL_TEXTURE_2D,
    texture3d = GL_TEXTURE_3D,
    texture1d_array = GL_TEXTURE_1D_ARRAY,
    texture2d_array = GL_TEXTURE_2D_ARRAY,
    texture_cubemap = GL_TEXTURE_CUBE_MAP,
    texture_cubemap_array = GL_TEXTURE_CUBE_MAP_ARRAY,
  };
  enum class warping : GLenum {
    repeat = GL_REPEAT,
    mirrored_repeat = GL_MIRRORED_REPEAT,
    clamp_to_edge = GL_CLAMP_TO_EDGE,
    clamp_to_border = GL_CLAMP_TO_BORDER,
  };
  enum class filtering : GLenum {
    nearest = GL_NEAREST,
    linear = GL_LINEAR,
    nearest_mipmap_nearest = GL_NEAREST_MIPMAP_NEAREST,
    linear_mipmap_nearest = GL_LINEAR_MIPMAP_NEAREST,
    nearest_mipmap_linear = GL_NEAREST_MIPMAP_LINEAR,
    linear_mipmap_linear = GL_LINEAR_MIPMAP_LINEAR,
  };

public:
  explicit textures(glfunctions *fun, type t, size_t n);
  explicit textures(glfunctions *fun, GLuint *ts, size_t n);
  virtual ~textures();

  textures(const textures &) = delete;
  textures(textures &&fb);
  textures &operator=(const textures &) = delete;
  textures &operator=(textures &&fb);

  void swap(textures &fb);  
  size_t size() const { return _size; }

  textures operator[](size_t i) const;

public:
  void set_warping_s(warping s) const;
  void set_warping_t(warping t) const;

  void set_border_color(const vec4f & color) const;

  void set_min_filtering(filtering min_f) const;
  void set_mag_filtering(filtering mag_f) const;

  void set_image(const image3u8 &im, bool generate_mipmap) const;

  void bind_to(int first_unit_id) const;

private:
  type _type;
  size_t _size;
  GLuint *_textures;
  bool _own_res;
};
}
}