#include <catch.hpp>

#include "scene.hpp"
#include "widget.hpp"

using namespace wheels;

TEST_CASE("scene simple", "[scene]") {

  polygonal_mesh<float, uint32_t, 3> mesh;
  mesh << sphere<vec3f, float>{ones<float>(3), 1.0f};

  qt::SceneWidget sw([](scene & s){
    
  });
  sw.show();
  if (qApp) {
    qApp->exec();
  }
}