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

#include "utility.hpp"

namespace wheels {
template <class E> class point_light {
public:
  vec_<E, 3> position;
  vec_<E, 3> color;
  E far_plane;
};

// make_point_light
template <class E, class S1, class N1, class T1, class S2, class N2, class T2>
constexpr auto
make_point_light(const tensor_base<E, tensor_shape<S1, N1>, T1> &position,
                 const tensor_base<E, tensor_shape<S2, N2>, T2> &color,
                 E far_plane = 20.0f) {
  return point_light<E>{position, color, far_plane};
}
}
