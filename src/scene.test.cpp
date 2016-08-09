#include <catch.hpp>

#include "scene.hpp"
#include "widget.hpp"

using namespace wheels;

TEST_CASE("scene multiple lights", "[scene][lights]") {
  int argc = 1;
  char * argv = "scene simple";
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
    s.add(point_light<float>{vec3f(3, 3, 3), vec3f(1, 1, 1)});
    s.add(point_light<float>{vec3f(5, -5, 3), vec3f(1, 0, 1)});
    s.add(point_light<float>{vec3f(-3, 3, -3), vec3f(1, 1, 1)});
    s.add(point_light<float>{vec3f(0, 7, -5), vec3f(1, 1, 1)});
    s.add(point_light<float>{vec3f(0, 7, 5), vec3f(1, 1, 1)});

    s.add("geo_big_box", (render_mesh<float, uint32_t, 3>() << box<vec3f>(
                              vec3f(-10, -10, -10), vec3f(10, 10, 10)))
                             .insided_out());

    s.add("geo_spheres", render_mesh<float, uint32_t, 3>()
                             << sphere<vec3f, float>{vec3f(0, 0, 2.0f), 1.0f}
                             << sphere<vec3f, float>{vec3f(-2, -2, 2.0f), 1.0f}
                             << sphere<vec3f, float>{vec3f(3, 3, 3.0f), 2.0f});

    s.add("mat_default",
          material{nullptr, nullptr, nullptr,
                   std::make_shared<image3f32>(image3f32(
                       make_shape(2, 2), {vec3f(1, 0, 0), vec3f(0, 0, 1),
                                          vec3f(0, 0, 1), vec3f(1, 0, 0)})),
                   nullptr});

    std::default_random_engine rng;
    s.add("mat_random", material{nullptr, nullptr, nullptr,
                                 std::make_shared<image3f32>(
                                     rand<vec3f>(make_shape(10, 10), rng)),
                                 nullptr});

    s.add_object("big_box", "geo_big_box", "mat_random");
    s.add_object("spheres", "geo_spheres", "mat_default");
  });
  sw.show();
  app.exec();
}