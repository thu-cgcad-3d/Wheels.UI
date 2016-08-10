#include "color.hpp"

namespace wheels {
namespace color {
vec3f hsv(float h, float s, float v) {
  int i;
  float f, p, q, t;
  if (s == 0.0) {
    // achromatic (grey)
    return vec3f(v, v, v);
  }
  h *= 6.0f; // sector 0 to 5
  i = std::floor(h);
  f = h - i; // factorial part of h
  p = v * (1 - s);
  q = v * (1 - s * f);
  t = v * (1 - s * (1 - f));
  switch (i) {
  case 0:
    return vec3f(v, t, p);
  case 1:
    return vec3f(q, v, p);
  case 2:
    return vec3f(p, v, t);
  case 3:
    return vec3f(p, q, v);
  case 4:
    return vec3f(t, p, v);
  default:
    return vec3f(v, p, q);
  }
}

namespace detail {
float _jet_interpolate(float val, float y0, float x0, float y1, float x1) {
  return (val - x0) * (y1 - y0) / (x1 - x0) + y0;
}
float _jet_base(float val) {
  if (val <= -0.75)
    return 0;
  else if (val <= -0.25)
    return _jet_interpolate(val, 0.0, -0.75, 1.0, -0.25);
  else if (val <= 0.25)
    return 1.0;
  else if (val <= 0.75)
    return _jet_interpolate(val, 1.0, 0.25, 0.0, 0.75);
  else
    return 0.0;
}
float _jet_red(float gray) { return _jet_base(gray - 0.5); }
float _jet_green(float gray) { return _jet_base(gray); }
float _jet_blue(float gray) { return _jet_base(gray + 0.5); }
}
vec3f jet(float lambda) {
  return vec3f(detail::_jet_red(lambda), detail::_jet_green(lambda),
               detail::_jet_blue(lambda));
}
}
}
