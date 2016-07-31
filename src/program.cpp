#include "program.hpp"

namespace wheels {
namespace opengl {
program::program(glfunctions *fun, const std::vector<shader *> &shaders)
    : object(fun) {
  _program = glfun->glCreateProgram();
  for (auto &shader : shaders) {
    glfun->glAttachShader(_program, shader->handle());
  }
  glfun->glLinkProgram(_program);
  glfun->glValidateProgram(_program);
}
program::~program() { glfun->glDeleteProgram(_program); }
}
}