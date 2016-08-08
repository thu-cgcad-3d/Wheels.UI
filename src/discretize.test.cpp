#include <catch.hpp>

#include "discretize.hpp"

using namespace wheels;

TEST_CASE("discretize sphere", "[discretize][sphere]") {
  polygonal_mesh<double, int, 3> pm;
  pm << sphere<vec3, double>{vec3(1, 2, 3), 3.0};

  box<vec3f> b(vec3f(-10, -10, -10), vec3f(10, 10, 10));
  auto b2 = bounding_box(polygonal_mesh<float, uint32_t, 3>() << b);
  REQUIRE(b2 == b);
}