#include "qt.hpp"

namespace wheels {
qt_opengl_manager::qt_opengl_manager() {
  glfun = new QOpenGLFunctions(QOpenGLContext::currentContext());
}

qt_opengl_manager::qt_opengl_manager(QOpenGLContext *context) {
  glfun = new QOpenGLFunctions(context);
}

qt_opengl_manager::~qt_opengl_manager() { delete glfun; }

void qt_opengl_manager::initialize_opengl() const {
  glfun->initializeOpenGLFunctions();
}

std::unique_ptr<opengl::shader>
qt_opengl_manager::create_shader(opengl::shader::type t,
                                 const char *src) const {
  return std::make_unique<opengl::shader>(glfun, t, src);
}
std::unique_ptr<opengl::program> qt_opengl_manager::create_program(
    const std::vector<opengl::shader *> &shaders) const {
  return std::make_unique<opengl::program>(glfun, shaders);
}
}
