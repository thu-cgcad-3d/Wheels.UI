#pragma once

#include "discretize_fwd.hpp"

#include "render_mesh.hpp"

namespace wheels {
// render_mesh
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const render_mesh<E, IndexT, 3> &mesh2) {
  IndexT cur_nverts = mesh.verts().numel();
  for (auto &&v : mesh2.verts()) {
    mesh.add_vert(v);
  }
  for (auto &&inds : mesh2.indices()) {
    mesh.add_indices(inds + cur_nverts);
  }
}

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

// disk
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const disk<vec_<E, 3>, E, vec_<E, 3>> &d) {
  discretize(mesh, make_polygon(d, 32));
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
  const int m = 128;
  const int n = 64;
  IndexT cur_nverts = (IndexT)mesh.verts().numel();
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      E xratio = 1.0f / n * j;
      E yratio = 1.0f / (m - 1) * i;
      E xangle = numeric_<E>::PI * 2 * xratio;
      E yangle = numeric_<E>::PI * yratio - numeric::PI_HALF;
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

// cone
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const cone<polygon<vec_<E, 3>, vec_<E, 3>>, vec_<E, 3>> &c) {
  discretize(mesh, c.base_shape);
  const auto &base_corners = c.base_shape.corners;
  for (int i = 0; i < base_corners.numel(); i++) {
    int next = (i + 1) % base_corners.numel();
    discretize(mesh, make_polygon(vecx_<vec_<E, 3>>(
                         {base_corners[next], base_corners[i], c.apex})));
  }
}

// cylinder
template <class E, class IndexT>
void discretize(
    render_mesh<E, IndexT, 3> &mesh,
    const cylinder<polygon<vec_<E, 3>, vec_<E, 3>>, vec_<E, 3>> &c) {
  discretize(mesh, c.base_shape);
  polygon<vec_<E, 3>> top_shape = c.base_shape;
  top_shape.normal = -top_shape.normal;
  for (auto &p : top_shape.corners) {
    p += c.offset;
  }
  const auto &base_corners = c.base_shape.corners;
  const auto &top_corners = top_shape.corners;
  for (int i = 0; i < base_corners.numel(); i++) {
    int next = (i + 1) % base_corners.numel();
    discretize(mesh, make_polygon(vecx_<vec_<E, 3>>(
                         {base_corners[next], base_corners[i], top_corners[i],
                          top_corners[next]})));
  }
  std::reverse(top_shape.corners.begin(), top_shape.corners.end());
  discretize(mesh, top_shape);
}
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
  const cylinder<disk<vec_<E, 3>, E, vec_<E, 3>>, vec_<E, 3>> &cld) {
  // base shape
  auto base_shape = make_polygon(cld.base_shape, 32);
  // add the bottom face
  discretize(mesh, base_shape);
  // top shape
  polygon<vec_<E, 3>> top_shape = base_shape;
  top_shape.normal = -top_shape.normal;
  for (auto &p : top_shape.corners) {
    p += cld.offset;
  }
  // add the side faces
  IndexT base_vert_start = mesh.verts().numel();
  for (int i = 0; i < base_shape.corners.numel(); i++) {
    auto &c = base_shape.corners[i];
    mesh.emplace_vert(c, (c - cld.base_shape.center).normalized());
  }
  IndexT top_vert_start = mesh.verts().numel();
  for (int i = 0; i < top_shape.corners.numel(); i++) {
    auto &c = top_shape.corners[i];
    mesh.emplace_vert(c,
                      (c - (cld.base_shape.center + cld.offset)).normalized());
  }
  for (int i = 0; i < base_shape.corners.numel(); i++) {
    int next = (i + 1) % base_shape.corners.numel();
    mesh.add_polygon(vec_<IndexT, 4>(base_vert_start + next,
                                     base_vert_start + i, top_vert_start + i,
                                     top_vert_start + next));
  }
  // add the top face
  std::reverse(top_shape.corners.begin(), top_shape.corners.end());
  discretize(mesh, top_shape);
}
}
