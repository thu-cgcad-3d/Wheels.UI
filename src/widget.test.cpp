#include <catch.hpp>


#include "scene.hpp"
#include "widget.hpp"

using namespace wheels;


//
//
//
//class TestWidget : public QOpenGLWidget, public QOpenGLExtraFunctions {
//public:
//  TestWidget() : QOpenGLWidget() {

//  }
//  virtual ~ TestWidget() {}
//
//protected:
//  void initializeGL() override{
//    makeCurrent();
//    initializeOpenGLFunctions();
//    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
//     // Setup some OpenGL options
//    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE);
//
//     // Setup and compile our shaders
//    Shader shader("point_shadows.vs", "point_shadows.frag", nullptr, context());
//    Shader simpleDepthShader("point_shadows_depth.vs", "point_shadows_depth.frag", "point_shadows_depth.gs", context());
//
//    // Set texture samples
//    shader.Use();
//    glUniform1i(glGetUniformLocation(shader.Program, "diffuseTexture"), 0);
//    glUniform1i(glGetUniformLocation(shader.Program, "depthMap"), 1);
//
//
//  }
//  void paintGL() override{
//    makeCurrent();
//
//  }
//  void resizeGL(int w, int h) override {
//    makeCurrent();
//
//  }
//};

TEST_CASE("OpenGL test", "[OpenGL]") {
  //TestWidget w;
  //w.show();
  //if (qApp) {
  //  qApp->exec();
  //}
}