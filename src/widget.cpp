#include "widget.hpp"

namespace wheels {
namespace qt {
SceneWidget::SceneWidget(std::function<void(scene &s)> f) : _init_fun(f) {
  QTimer *timer = new QTimer(this);
  connect(timer, SIGNAL(timeout()), this, SLOT(update()));
  timer->start(100);
}

void SceneWidget::initializeGL() {
  _scene.setup(context()->versionFunctions<opengl::glfunctions>());
  _scene.camera() =
      perspective_camera<float>(vec3f(7, 0, 1), vec3f(0, 0, 0), vec3f(0, 1, 0),
                                500, 500, 250, vec2f(250, 250), 0.001, 1e4);

  _scene.add("default light", point_light<float>{vec3f(3, 3, 5)});
  _scene.add("geo_ground", polygonal_mesh<float, uint32_t, 3>() << box<vec3f>(
                               vec3f(-10, -10, -10), vec3f(10, 10, 10)));

  _scene.add("geo_spheres",
             polygonal_mesh<float, uint32_t, 3>()
                 << sphere<vec3f, float>{vec3f(0, 0, 2.0f), 1.0f}
                 << sphere<vec3f, float>{vec3f(-2, -2, 2.0f), 1.0f}
                 << sphere<vec3f, float>{vec3f(2, 2, 2.0f), 1.0f});
  
  _scene.add("mat_default",
             material{nullptr, nullptr, nullptr,
                      std::make_shared<image3f32>(make_shape(1, 1),
                                                  vec3f(1.0, 0.5, 1.0)),
                      nullptr});
  _scene.add_object("ground", "geo_ground", "mat_default");
  _scene.add_object("spheres", "geo_spheres", "mat_default");
  if (_init_fun) {
    _init_fun(_scene);
  }
}

void SceneWidget::resizeGL(int w, int h) {
  _scene.camera().resize_screen(w, h);
}
void SceneWidget::paintGL() {
  _scene._name2obj["ground"].model_matrix =
      rotate(_scene._name2obj["ground"].model_matrix,
             (float)numeric::PI / 100.0f, vec3f(0, 0, 1));
  _scene.render(defaultFramebufferObject()); 
}
}
}