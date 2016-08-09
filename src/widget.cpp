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

  _scene.add("default_light", point_light<float>{vec3f(3, 3, 3)});

  render_mesh<float, uint32_t, 3> box_mesh;
  box_mesh << box<vec3f>(vec3f(-10, -10, -10), vec3f(10, 10, 10));
  box_mesh.for_each_indices(
      [](auto &inds) { std::reverse(inds.begin(), inds.end()); });
  box_mesh.for_each_vert([](auto &v) { v.normal = -v.normal; });
  _scene.add("geo_ground", box_mesh);

  _scene.add("geo_spheres",
             render_mesh<float, uint32_t, 3>()
                 << sphere<vec3f, float>{vec3f(0, 0, 2.0f), 1.0f}
                 << sphere<vec3f, float>{vec3f(-2, -2, 2.0f), 1.0f}
                 << sphere<vec3f, float>{vec3f(2, 2, 2.0f), 1.0f});

  _scene.add("mat_default",
             material{nullptr, nullptr, nullptr,
                      std::make_shared<image3f32>(image3f32(
                          make_shape(2, 2), {vec3f(1, 0, 0), vec3f(0, 0, 1),
                                             vec3f(0, 0, 1), vec3f(1, 0, 0)})),
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
  static double t = 0;
  _scene._name2light["default_light"].position =
      vec3f(2 * std::sin(t), 2 * std::cos(t), 8.0);
  _scene._name2obj["ground"].model_matrix =
      rotate(_scene._name2obj["ground"].model_matrix,
             (float)numeric::PI / 1000.0f, vec3f(0, 0, 1));
  _scene.render(defaultFramebufferObject()); 
  t += 0.01;
}
}
}