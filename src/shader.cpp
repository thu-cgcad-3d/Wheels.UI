#include "shader.hpp"

#include "wheels/geometry.hpp"

namespace wheels {
namespace opengl {
shader::shader(glfunctions *fun, type t, const char *src) : object(fun) {
  _shader = glfun->glCreateShader(to_underlying_type(t));
  glfun->glShaderSource(_shader, 1, &src, nullptr);
  glfun->glCompileShader(_shader);
}

shader::~shader() { glfun->glDeleteShader(_shader); }
}
}
