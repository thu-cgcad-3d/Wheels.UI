#include "glrenderbuffers.hpp"

namespace wheels {
namespace opengl {
renderbuffers::renderbuffers(glfunctions *fun, size_t n)
    : object(fun), _size(n), _rbs(new GLuint[n]), _own_res(true) {
  fun->glGenRenderbuffers(n, _rbs);
}
renderbuffers::renderbuffers(glfunctions *fun, GLuint *fbs, size_t n)
    : object(fun), _size(n), _rbs(fbs), _own_res(false) {}
renderbuffers::~renderbuffers() {
  if (_own_res) {
    glfun->glDeleteRenderbuffers(_size, _rbs);
    delete[] _rbs;
  }
}
renderbuffers::renderbuffers(renderbuffers &&fb) : object() { swap(fb); }
renderbuffers &renderbuffers::operator=(renderbuffers &&fb) {
  swap(fb);
  return *this;
}
void renderbuffers::swap(renderbuffers &fb) {
  object::swap(fb);
  std::swap(fb._size, _size);
  std::swap(fb._rbs, _rbs);
  std::swap(_own_res, fb._own_res);
}
renderbuffers renderbuffers::operator[](size_t i) const {
  return renderbuffers(glfun, _rbs + i, 1);
}
}
}
