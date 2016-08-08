#pragma once

#include "scene.hpp"

namespace wheels {
namespace qt {
class SceneWidget : public QOpenGLWidget {
public:
  explicit SceneWidget(std::function<void(scene &s)> f);

protected:
  virtual void initializeGL() override;
  virtual void resizeGL(int w, int h) override;
  virtual void paintGL() override;

private:
  scene _scene;
  std::function<void(scene &s)> _init_fun;
};
}
}