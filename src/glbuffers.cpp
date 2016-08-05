#include "glbuffers.hpp"

namespace wheels {
namespace opengl {
buffers::buffers(glfunctions *fun, size_t n)
    : object(fun), _size(n), _buffers(new GLuint[n]), _own_res(true) {
  fun->glGenBuffers(n, _buffers);
}
buffers::buffers(glfunctions *fun, GLuint *fbs, size_t n)
    : object(fun), _size(n), _buffers(fbs), _own_res(false) {}
buffers::buffers(glfunctions *fun, type t, void *ptr, size_t length,
                 data_usage usage)
    : object(fun), _size(1), _buffers(new GLuint[1]), _own_res(true) {
  fun->glGenBuffers(1, _buffers);
  glfun->glBindBuffer(to_underlying_type(_type), _buffers[0]);
  glfun->glBufferData(to_underlying_type(_type), length, ptr,
                      to_underlying_type(usage));
}

buffers::~buffers() {
  if (_own_res) {
    glfun->glDeleteBuffers(_size, _buffers);
    delete[] _buffers;
  }
}
buffers::buffers(buffers &&fb) : object() { swap(fb); }
buffers &buffers::operator=(buffers &&fb) {
  swap(fb);
  return *this;
}

void buffers::swap(buffers &fb) {
  object::swap(fb);
  std::swap(fb._size, _size);
  std::swap(fb._buffers, _buffers);
  std::swap(_own_res, fb._own_res);
}

buffers buffers::operator[](size_t i) const {
  assert(i < _size);
  return buffers(glfun, _buffers + i, 1);
}

void buffers::set_data(void *ptr, size_t length, data_usage usage,
                       size_t i) const {
  glfun->glBindBuffer(to_underlying_type(_type), _buffers[i]);
  glfun->glBufferData(to_underlying_type(_type), length, ptr,
                      to_underlying_type(usage));
}

uint8_t const *buffers::ptr(size_t i) const {
  glfun->glBindBuffer(to_underlying_type(_type), _buffers[i]);
  constexpr GLuint valid_types[] = {GL_ARRAY_BUFFER,
                                    GL_COPY_READ_BUFFER,
                                    GL_COPY_WRITE_BUFFER,
                                    GL_ELEMENT_ARRAY_BUFFER,
                                    GL_PIXEL_PACK_BUFFER,
                                    GL_PIXEL_UNPACK_BUFFER,
                                    GL_TRANSFORM_FEEDBACK_BUFFER,
                                    GL_UNIFORM_BUFFER};
  assert(std::find(std::begin(valid_types), std::end(valid_types),
                   to_underlying_type(_type)) != std::end(valid_types));
  return (uint8_t const *)glfun->glMapBufferRange(to_underlying_type(_type), 0,
                                                  nbytes(i), GL_MAP_READ_BIT);
}
size_t buffers::nbytes(size_t i) const {
  int n = 0;
  glfun->glBindBuffer(to_underlying_type(_type), _buffers[i]);
  glfun->glGetBufferParameteriv(to_underlying_type(_type), GL_BUFFER_SIZE, &n);
  return (size_t)n;
}
buffers::data_usage buffers::usage(size_t i) const {
  int u = 0;
  glfun->glBindBuffer(to_underlying_type(_type), _buffers[i]);
  glfun->glGetBufferParameteriv(to_underlying_type(_type), GL_BUFFER_USAGE, &u);
  return (buffers::data_usage)u;
}
}
}