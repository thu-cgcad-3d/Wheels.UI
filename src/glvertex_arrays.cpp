#include "glvertex_arrays.hpp"

namespace wheels {
namespace opengl {
vertex_arrays::vertex_arrays(glfunctions *fun, size_t n)
    : object(fun), _size(n), _vaos(new GLuint[n]), _own_res(true) {
  fun->glGenVertexArrays(n, _vaos);
}
vertex_arrays::vertex_arrays(glfunctions *fun, GLuint *fbs, size_t n)
    : object(fun), _size(n), _vaos(fbs), _own_res(false) {}
vertex_arrays::~vertex_arrays() {
  if (_own_res) {
    glfun->glDeleteVertexArrays(_size, _vaos);
    delete[] _vaos;
  }
}
vertex_arrays::vertex_arrays(vertex_arrays &&fb) : object() { swap(fb); }
vertex_arrays &vertex_arrays::operator=(vertex_arrays &&fb) {
  swap(fb);
  return *this;
}
void vertex_arrays::swap(vertex_arrays &fb) {
  object::swap(fb);
  std::swap(fb._size, _size);
  std::swap(fb._vaos, _vaos);
  std::swap(_own_res, fb._own_res);
}
vertex_arrays vertex_arrays::operator[](size_t i) const {
  assert(i < _size);
  return vertex_arrays(glfun, _vaos + i, 1);
}

}
}
