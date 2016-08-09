#include <catch.hpp>

#include "render_mesh.hpp"

using namespace wheels;

TEST_CASE("polygonal mesh", "[render_mesh]") {
  render_mesh<double, int, 3> pm;
  pm.emplace_vert(vec3(0, 0, 0));
  pm.emplace_vert(vec3(0, 1, 0));
  pm.emplace_vert(vec3(1, 1, 0));
  pm.emplace_vert(vec3(1, 0, 0));

  pm.add_polygon(iota<int>(4));
  REQUIRE(pm.indices().numel() == 2);

  auto bb = bounding_box(pm);
  REQUIRE(bb == bounding_box_in({vec3(0, 0, 0), vec3(1, 1, 0)}));
}