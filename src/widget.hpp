#pragma once

#include "scene.hpp"

namespace wheels {
namespace qt {
void show(std::function<void(scene &s)> f);

class SceneWidget : public QOpenGLWidget {
public:
  explicit SceneWidget(std::function<void(scene &s)> f);

protected:
  virtual void initializeGL() override;
  virtual void resizeGL(int w, int h) override;
  virtual void paintGL() override;

  virtual void mousePressEvent(QMouseEvent *e) override;
  virtual void mouseMoveEvent(QMouseEvent *e) override;
  virtual void mouseReleaseEvent(QMouseEvent *e) override;
  virtual void wheelEvent(QWheelEvent *e) override;

private:
  std::unique_ptr<scene> _scene;
  std::function<void(scene &s)> _init_fun;
  QPointF _last_pos;
};
}
}