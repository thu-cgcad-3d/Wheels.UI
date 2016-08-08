#pragma once

#include "wheels/geometry.hpp"

namespace wheels {
template <class E> struct polygonal_mesh_vertex {
  vec_<E, 3> position, normal;
  vec_<E, 2> tex_coord;
};

template <class E>
box<vec_<E, 3>> bounding_box(const polygonal_mesh_vertex<E> &v) {
  return bounding_box(v.position);
}

template <class E, class IndexT, size_t IndexSize> class polygonal_mesh {
public:
  using vertex = polygonal_mesh_vertex<E>;

public:
  auto verts() const { return proxy(_verts); }
  auto indices() const { return proxy(_indices); }

  const E *verts_ptr() const { return (const E *)(_verts.data()); }
  const IndexT *indices_ptr() const {
    return (const IndexT *)(_indices.data());
  }

  size_t nbytes_of_verts() const { return _verts.size() * sizeof(vertex); }
  size_t nbytes_of_indices() const {
    return _indices.size() * IndexSize * sizeof(IndexT);
  }

  // add_vert
  template <class VT> IndexT add_vert(VT &&v) {
    _verts.push_back(std::forward<VT>(v));
  }

  // emplace_vert
  template <class VT1, class VT2 = vec_<E, 3>, class VT3 = vec_<E, 2>>
  IndexT emplace_vert(VT1 &&position, VT2 &&normal = VT2(),
                      VT3 &&tex_coord = VT3()) {
    _verts.push_back(vertex{std::forward<VT1>(position),
                            std::forward<VT2>(normal),
                            std::forward<VT3>(tex_coord)});
    return (IndexT)_verts.size() - 1;
  }

  // add_indices
  template <class S, class N, class T>
  void add_indices(
      const tensor_base<IndexT, tensor_shape<S, N>, T> &corner_inds_base) {
    _indices.push_back(vec_<IndexT, IndexSize>(corner_inds_base));
  }

  // add_polygon
  template <size_t I = IndexSize, class S, class N, class T>
  std::enable_if_t<I == 3> add_polygon(
      const tensor_base<IndexT, tensor_shape<S, N>, T> &corner_inds_base) {
    decltype(auto) corner_inds = corner_inds_base.derived();
    assert(corner_inds.numel() >= 3);
    assert(_verts.size() >= 3);
    vec_<E, 3> o = _verts[corner_inds[0]].position;
    vec_<E, 3> normal_for_triangulation = normal_of_triangle(
        o, _verts[corner_inds[1]].position, _verts[corner_inds[2]].position);
    vec_<E, 3> x, y;
    propose_xy_from_z(x, y, normal_for_triangulation);

    auto triangle_corner_inds = triangulate<IndexT>(
        verts().ewised().transform([&o, &x, &y](const vertex &v) -> vec_<E, 2> {
          return vec_<E, 2>((v.position - o).dot(x), (v.position - o).dot(y));
        })[corner_inds]);
    for (auto &&t : triangle_corner_inds) {
      _indices.push_back(corner_inds[t]);
    }
  }

  // discretize
  template <class T> polygonal_mesh &operator<<(const T &s) & {
    discretize(*this, s);
    return *this;
  }
  template <class T> polygonal_mesh &&operator<<(const T &s) && {
    discretize(*this, s);
    return std::move(*this);
  }

private:
  std::vector<vertex> _verts;
  std::vector<vec_<IndexT, IndexSize>> _indices;
};

template <class E, class IndexT, size_t IndexSize>
box<vec_<E, 3>> bounding_box(const polygonal_mesh<E, IndexT, IndexSize> &mesh) {
  return bounding_box_in(mesh.verts());
}
}
