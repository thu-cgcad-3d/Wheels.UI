#include "widget.hpp"

namespace wheels {
namespace qt {
SceneWidget::SceneWidget(std::function<void(scene &s)> f) : _init_fun(f) {
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(10);
}

void SceneWidget::initializeGL() {
  _scene.setup(context()->versionFunctions<opengl::glfunctions>());
  _scene.camera() =
      perspective_camera<float>(vec3f(7, -7, 7), vec3f(0, 0, 0), vec3f(0, 1, 0),
                                500, 500, 250, vec2f(250, 250), 0.001, 1e4);
  if (_init_fun) {
    _init_fun(_scene);
  }
  _scene.init(defaultFramebufferObject());
}

void SceneWidget::resizeGL(int w, int h) {
  _scene.camera().resize_screen(w, h);
}
void SceneWidget::paintGL() {
  static double t = 0;
  _scene._name2obj["ground"].model_matrix =
      rotate(_scene._name2obj["ground"].model_matrix,
             (float)numeric::PI / 1000.0f, vec3f(0, 0, 1));
  _scene.render(defaultFramebufferObject()); 
  t += 0.01;
}
}
}