#include "gltextures.hpp"

namespace wheels {
namespace opengl {
textures::textures(glfunctions *fun, type t, size_t n)
    : object(fun), _size(n), _textures(new GLuint[n]), _own_res(true),
      _type(t) {
  fun->glGenTextures(n, _textures);
  // The target parameter of glBindTexture corresponds to the texture's type.
  // So when you use a freshly generated texture name, the first bind helps
  // define the type of the texture. It is not legal to bind an object to a
  // different target than the one it was previously bound with. So if you
  // generate a texture and bind it as GL_TEXTURE_1D, then you must continue to
  // bind it as such.
  for (int i = 0; i < n; i++) {
    fun->glBindTexture(to_underlying_type(t), _textures[i]);
  }
}
textures::textures(glfunctions *fun, GLuint *ts, size_t n)
    : object(fun), _size(n), _textures(ts), _own_res(false) {}
textures::~textures() {
  if (_own_res) {
    glfun->glDeleteTextures(_size, _textures);
    delete[] _textures;
  }
}
textures::textures(textures &&t) : object() { swap(t); }
textures &textures::operator=(textures &&t) {
  swap(t);
  return *this;
}
void textures::swap(textures &t) {
  object::swap(t);
  std::swap(t._size, _size);
  std::swap(t._textures, _textures);
  std::swap(_own_res, t._own_res);
}

textures textures::operator[](size_t i) const {
  return textures(glfun, _textures + i, 1);
}

void textures::set_warping_s(warping s) const {
  for (int i = 0; i < _size; i++) {
    glfun->glBindTexture(to_underlying_type(_type), _textures[i]);
    glfun->glTexParameteri(to_underlying_type(_type), GL_TEXTURE_WRAP_S,
                           to_underlying_type(s));
  }
  glfun->glBindTexture(to_underlying_type(_type), 0);
}

void textures::set_warping_t(warping t) const {
  for (int i = 0; i < _size; i++) {
    glfun->glBindTexture(to_underlying_type(_type), _textures[i]);
    glfun->glTexParameteri(to_underlying_type(_type), GL_TEXTURE_WRAP_T,
                           to_underlying_type(t));
  }
  glfun->glBindTexture(to_underlying_type(_type), 0);
}

void textures::set_border_color(const vec4f &color) const {
  for (int i = 0; i < _size; i++) {
    glfun->glBindTexture(to_underlying_type(_type), _textures[i]);
    glfun->glTexParameterfv(to_underlying_type(_type), GL_TEXTURE_BORDER_COLOR,
                            color.ptr());
  }
  glfun->glBindTexture(to_underlying_type(_type), 0);
}
void textures::set_min_filtering(filtering min_f) const {
  for (int i = 0; i < _size; i++) {
    glfun->glBindTexture(to_underlying_type(_type), _textures[i]);
    glfun->glTexParameteri(to_underlying_type(_type), GL_TEXTURE_MIN_FILTER,
                           to_underlying_type(min_f));
  }
  glfun->glBindTexture(to_underlying_type(_type), 0);
}
void textures::set_mag_filtering(filtering mag_f) const {
  for (int i = 0; i < _size; i++) {
    glfun->glBindTexture(to_underlying_type(_type), _textures[i]);
    glfun->glTexParameteri(to_underlying_type(_type), GL_TEXTURE_MAG_FILTER,
                           to_underlying_type(mag_f));
  }
  glfun->glBindTexture(to_underlying_type(_type), 0);
}
void textures::set_image(const image3u8 &im, bool generate_mipmap) const {
  for (int i = 0; i < _size; i++) {
    glfun->glBindTexture(to_underlying_type(_type), _textures[i]);
    glfun->glTexImage2D(to_underlying_type(_type), 0, GL_RGB, im.width(),
                        im.height(), 0, GL_RGB, GL_UNSIGNED_BYTE,
                        (const uint8_t *)im.ptr());
    if (generate_mipmap) {
      glfun->glGenerateMipmap(to_underlying_type(_type));
    }
  }
  glfun->glBindTexture(to_underlying_type(_type), 0);
}
void textures::bind_to(int first_unit_id) const {
  for (int i = 0; i < _size; i++) {
    glfun->glActiveTexture(GL_TEXTURE0 + first_unit_id + i);
    glfun->glBindTexture(to_underlying_type(_type), _textures[i]);
  }
}
}
}
