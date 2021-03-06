#pragma once

#include "wheels/tensor_fwd.hpp"

namespace wheels {
template <class PointT> class box;
template <class PointT, class DirT> class polygon;
template <class PointT, class E, class DirT> class disk;
template <class PointT, class T> class sphere;
template <class BaseShapeT, class PointT> class cone;
template <class BaseShapeT, class OffsetT> class cylinder;

template <class E, class IndexT, size_t IndexSize> class render_mesh;

// render_mesh
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const render_mesh<E, IndexT, 3> &mesh2);

// polygon
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const polygon<vec_<E, 3>, vec_<E, 3>> &p);

// disk
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const disk<vec_<E, 3>, E, vec_<E, 3>> &p);

// box
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh, const box<vec_<E, 3>> &b);

// sphere
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh, const sphere<vec_<E, 3>, E> &s,
                int m = 64, int n = 32);

// cone
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const cone<polygon<vec_<E, 3>, vec_<E, 3>>, vec_<E, 3>> &c);

// cylinder
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const cylinder<polygon<vec_<E, 3>, vec_<E, 3>>, vec_<E, 3>> &c);
template <class E, class IndexT>
void discretize(render_mesh<E, IndexT, 3> &mesh,
                const cylinder<disk<vec_<E, 3>, E, vec_<E, 3>>, vec_<E, 3>> &c);
}
