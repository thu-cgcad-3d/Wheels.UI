#pragma once

#include "render_mesh.hpp"

namespace wheels {
// polygon
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const polygon<vec_<E, 3>, vec_<E, 3>> &p) {
  IndexT cur_nverts = mesh.verts().numel();
  for (int i = 0; i < p.corners.numel(); i++) {
    mesh.emplace_vert(p.corners[i], p.normal);
  }
  mesh.add_polygon(iota<IndexT>(p.corners.numel()) + cur_nverts);
}

// box
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh, const box<vec_<E, 3>> &b) {
  if (b.is_null()) {
    return;
  }
  const auto center = b.center();
  vec_<E, 3> corners[8];
  int count = 0;
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 2; j++) {
      for (int k = 0; k < 2; k++) {
        vec_<E, 3> c((i == 1 ? b.max_corner() : b.min_corner())[0],
                     (j == 1 ? b.max_corner() : b.min_corner())[1],
                     (k == 1 ? b.max_corner() : b.min_corner())[2]);
        corners[count++] = c;
      }
    }
  }
  discretize(mesh, make_polygon(vecx_<vec_<E, 3>>(
                       {corners[0], corners[4], corners[5], corners[1]})));
  discretize(mesh, make_polygon(vecx_<vec_<E, 3>>(
                       {corners[4], corners[6], corners[7], corners[5]})));
  discretize(mesh, make_polygon(vecx_<vec_<E, 3>>(
                       {corners[6], corners[2], corners[3], corners[7]})));
  discretize(mesh, make_polygon(vecx_<vec_<E, 3>>(
                       {corners[2], corners[0], corners[1], corners[3]})));
  discretize(mesh, make_polygon(vecx_<vec_<E, 3>>(
                       {corners[1], corners[5], corners[7], corners[3]})));
  discretize(mesh, make_polygon(vecx_<vec_<E, 3>>(
                       {corners[2], corners[6], corners[4], corners[0]})));
}

// sphere
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const sphere<vec_<E, 3>, E> &s) {
  int m = 128;
  int n = 64;
  IndexT cur_nverts = mesh.verts().numel();
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      float xratio = 1.0f / n * j;
      float yratio = 1.0f / (m - 1) * i;
      float xangle = numeric::PI * 2 * xratio;
      float yangle = numeric::PI * yratio - numeric::PI_HALF;
      vec_<E, 3> normal(std::cos(xangle) * std::cos(yangle),
                        std::sin(xangle) * std::cos(yangle), std::sin(yangle));
      vec_<E, 3> position = normal * s.radius + s.center;
      vec_<E, 2> tex_coord(xratio, yratio);
      mesh.emplace_vert(position, normal, tex_coord);
    }
  }
  auto vids = iota<IndexT>(m * n).reshaped(make_shape(m, n)) + cur_nverts;
  for (int i = 1; i < m; i++) {
    int previ = i == 0 ? m - 1 : i - 1;
    for (int j = 0; j < n; j++) {
      int prevj = j == 0 ? n - 1 : j - 1;
      mesh.add_indices(
          vec_<IndexT, 3>(vids(i, j), vids(i, prevj), vids(previ, prevj)));
      mesh.add_indices(
          vec_<IndexT, 3>(vids(i, j), vids(previ, prevj), vids(previ, j)));
    }
  }
}
}
