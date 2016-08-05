#include "glmanager.hpp"

namespace wheels {
namespace opengl {
manager::manager() {
  glfun = new glfunctions(QOpenGLContext::currentContext());
}

manager::manager(QOpenGLContext *context) {
  glfun = new glfunctions(context);
}

manager::~manager() { delete glfun; }

void manager::initialize_opengl() const { glfun->initializeOpenGLFunctions(); }

std::unique_ptr<shader> manager::create_shader(shader::type t,
                                               const char *src) const {
  return std::make_unique<shader>(glfun, t, src);
}
std::unique_ptr<program>
manager::create_program(const std::vector<shader *> &shaders) const {
  return std::make_unique<program>(glfun, shaders);
}
std::unique_ptr<textures> manager::generate_textures(textures::type t,
                                                     size_t n) const {
  return std::make_unique<textures>(glfun, t, n);
}
}
}
