#include "glframebuffers.hpp"

namespace wheels {
namespace opengl {
framebuffers::framebuffers(glfunctions *fun, size_t n)
    : object(fun), _size(n), _fbs(new GLuint[n]), _own_res(true) {
  fun->glGenFramebuffers(n, _fbs);
}
framebuffers::framebuffers(glfunctions *fun, GLuint *fbs, size_t n)
    : object(fun), _size(n), _fbs(fbs), _own_res(false) {}
framebuffers::~framebuffers() {
  if (_own_res) {
    glfun->glDeleteFramebuffers(_size, _fbs);
    delete[] _fbs;
  }
}
framebuffers::framebuffers(framebuffers &&fb) : object() { swap(fb); }
framebuffers &framebuffers::operator=(framebuffers &&fb) {
  swap(fb);
  return *this;
}
void framebuffers::swap(framebuffers &fb) {
  object::swap(fb);
  std::swap(fb._size, _size);
  std::swap(fb._fbs, _fbs);
  std::swap(_own_res, fb._own_res);
}
framebuffers framebuffers::operator[](size_t i) const {
  return framebuffers(glfun, _fbs + i, 1);
}
}
}
