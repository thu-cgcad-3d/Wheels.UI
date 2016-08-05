#include "glprogram.hpp"

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

  // extract all uniforms

}
program::~program() { glfun->glDeleteProgram(_program); }
GLuint program::uniform_block_index(const char *uniform_block_name) const {
  return glfun->glGetUniformBlockIndex(_program, uniform_block_name);
}
void program::texture_slot::bind(const textures & t) const {
  for (int i = 0; i < t.size(); i++) {
    glfun->glActiveTexture(GL_TEXTURE0 + slot_id + i);
    //glfun->glBindTexture(to_underlying_type(t.), _textures[i]);
  }
}
}
}