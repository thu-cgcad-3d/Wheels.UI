#include "widget.hpp"

#include "camera_manip.hpp"

namespace wheels {
namespace qt {
SceneWidget::SceneWidget(std::function<void(scene &s)> f)
    : _init_fun(f), _scene(std::make_unique<scene>()) {
  setMouseTracking(true);
  resize(600, 600);
}

void SceneWidget::initializeGL() {
  _scene->setup(context()->versionFunctions<opengl::glfunctions>());
  _scene->camera() =
      perspective_camera<float>(vec3f(7, -7, 7), vec3f(0, 0, 0), vec3f(0, 1, 0),
                                500, 500, 400, vec2f(250, 250), 0.001, 1e4);
  if (_init_fun) {
    _init_fun(*_scene);
  }
  _scene->init(defaultFramebufferObject());
}

void SceneWidget::resizeGL(int w, int h) {
  _scene->camera().resize_screen(w, h);
}
void SceneWidget::paintGL() { _scene->render(defaultFramebufferObject()); }
void SceneWidget::mousePressEvent(QMouseEvent *e) {
  _last_pos = e->pos();
  if (e->buttons() & Qt::RightButton)
    setCursor(Qt::OpenHandCursor);
  else if (e->buttons() & Qt::MidButton)
    setCursor(Qt::SizeAllCursor);
  else if (e->buttons() & Qt::LeftButton) {
  }
  update();
}
void SceneWidget::mouseMoveEvent(QMouseEvent *e) {
  QVector3D t(e->pos() - _last_pos);
  t.setX(-t.x());
  if ((e->buttons() & Qt::RightButton) &&
      !(e->modifiers() & Qt::ShiftModifier)) {
    vec3f trans =
        t.x() * _scene->camera().rightward() + t.y() * _scene->camera().up;
    trans *= 0.02f * distance(_scene->camera().eye, _scene->camera().center);
    manipulate(_scene->camera()).move_eye_around(trans, false);
    setCursor(Qt::ClosedHandCursor);
    update();
  } else if ((e->buttons() & Qt::MidButton) ||
             ((e->buttons() & Qt::RightButton) &&
              (e->modifiers() & Qt::ShiftModifier))) {
    vec3f trans =
        t.x() * _scene->camera().rightward() + t.y() * _scene->camera().up;
    trans *= 0.02f * distance(_scene->camera().eye, _scene->camera().center);
    manipulate(_scene->camera()).translate(trans);
    update();
  }
  _last_pos = e->pos();
}
void SceneWidget::mouseReleaseEvent(QMouseEvent *e) { unsetCursor(); }

void SceneWidget::wheelEvent(QWheelEvent *e) {
  float d = e->delta() / 10;
  float dist = distance(_scene->camera().eye, _scene->camera().center);
  vec3f trans = d * dist / 100.0f * _scene->camera().forward();
  _scene->camera().eye += trans;
  update();
}
}
}