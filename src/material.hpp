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

#include <memory>

#include "wheels/tensor_fwd.hpp"

#include "utility.hpp"

namespace wheels {
class material {
public:
  std::string compute_color_function;
  std::shared_ptr<image3f32> normal_map;
  std::shared_ptr<image3f32> ambient_map;
  std::shared_ptr<image3f32> diffuse_map;
  std::shared_ptr<image3f32> specular_map;
  std::shared_ptr<image1f32> alpha_map;

public:
  std::shared_ptr<image3f32> texture(
      const_ints<opengl::texture_attribute, opengl::texture_attribute::normal>)
      const;
  std::shared_ptr<image3f32> texture(
      const_ints<opengl::texture_attribute, opengl::texture_attribute::ambient>)
      const;
  std::shared_ptr<image3f32> texture(
      const_ints<opengl::texture_attribute, opengl::texture_attribute::diffuse>)
      const;
  std::shared_ptr<image3f32>
      texture(const_ints<opengl::texture_attribute,
                         opengl::texture_attribute::specular>) const;
  std::shared_ptr<image1f32> texture(
      const_ints<opengl::texture_attribute, opengl::texture_attribute::alpha>)
      const;
};

// make_pure_material
material make_pure_material(const vec3f &color);
material make_pure_material(image3f32 &&diffuse_map);

// make_simple_material
material make_simple_material(const vec3f &color);
material make_simple_material(image3f32 &&diffuse_map);
}
