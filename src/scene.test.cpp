#include <catch.hpp>

#include "scene.hpp"
#include "widget.hpp"

using namespace wheels;

TEST_CASE("scene multiple lights", "[scene][lights]") {
  int argc = 1;
  char *argv = "scene simple";
  QApplication app(argc, &argv);

  QSurfaceFormat format;
  format.setDepthBufferSize(24);
  format.setStencilBufferSize(8);
  format.setVersion(4, 3);
  format.setSamples(8);
  format.setProfile(QSurfaceFormat::CoreProfile);
  QSurfaceFormat::setDefaultFormat(format);

  qDebug("OpenGL version: %d.%d",
         QSurfaceFormat::defaultFormat().majorVersion(),
         QSurfaceFormat::defaultFormat().minorVersion());

  qt::SceneWidget sw([](scene &s) {
    s.add_light(point_light<float>{vec3f(3, 3, 5), color::white});
    s.add_light(point_light<float>{vec3f(-7, -5, 8), color::orange});
    s.add_light(point_light<float>{vec3f(0, 0, -5), color::white});

    s.add_material("white", color::white);
    s.add_material("light gray", color::light_gray);

    s.add_material("red", color::red);
    s.add_material("green", color::green);
    s.add_material("blue", color::blue);

    s.add_geometry("plate",
                   box<vec3f>(vec3f(-11, -11, -1.3), vec3f(11, 11, -1.0)));

    s.add_geometry(
        "xbar",
        make_cylinder_along(make_line(vec3f(0, 0, 0), vec3f(10, 0, 0)), 0.5f));
    s.add_geometry(
        "ybar",
        make_cylinder_along(make_line(vec3f(0, 0, 0), vec3f(0, 10, 0)), 0.5f));
    s.add_geometry(
        "zbar",
        make_cylinder_along(make_line(vec3f(0, 0, 0), vec3f(0, 0, 10)), 0.5f));

    s.add_geometry("ball", sphere<vec3f, float>{vec3f(), 1.0f});

    s.add_object("x", "xbar", "red");
    s.add_object("y", "ybar", "green");
    s.add_object("z", "zbar", "blue");
    s.add_object("o", "ball", "light gray");

    s.add_object("plate", "plate", "white");

    s.add_object("ox", "ball", "red",
                 make_transform().translate(vec3f(10, 0, 0)).matrix());
    s.add_object("oy", "ball", "green",
                 make_transform().translate(vec3f(0, 10, 0)).matrix());
    s.add_object("oz", "ball", "blue",
                 make_transform().translate(vec3f(0, 0, 10)).matrix());
  });
  sw.show();
  app.exec();
}