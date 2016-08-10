#pragma once

#include "wheels/tensor.hpp"

namespace wheels {
namespace color {
static constexpr vec3f black(0, 0, 0);
static constexpr vec3f white(1, 1, 1);

static constexpr vec3f gray(0.5, 0.5, 0.5);
static constexpr vec3f dim_gray(105.0 / 255.0, 105.0 / 255.0, 105.0 / 255.0);
static constexpr vec3f dark_gray(169.0 / 255.0, 169.0 / 255.0, 169.0 / 255.0);
static constexpr vec3f silver(192.0 / 255.0, 192.0 / 255.0, 192.0 / 255.0);
static constexpr vec3f light_gray(211.0 / 255.0, 211.0 / 255.0, 211.0 / 255.0);

static constexpr vec3f red(1, 0, 0);
static constexpr vec3f green(0, 1, 0);
static constexpr vec3f blue(0, 0, 1);

static constexpr vec3f yellow(1, 1, 0);
static constexpr vec3f magenta(1, 0, 1);
static constexpr vec3f cyan(0, 1, 1);

static constexpr vec3f orange(1, 165.0 / 255.0, 0);

// convert hsv color to rgb color
vec3f hsv(float h, float s, float v);

// generate a pseudo color from a scalar
vec3f jet(float lambda);
}
}
