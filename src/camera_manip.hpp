#pragma once

#include "wheels/geometry.hpp"

namespace wheels {
template <class E> class camera_manip {
public:
  explicit camera_manip(perspective_camera<E> &c) : _camera(c) {}

  void adjust_clip_planes(const sphere<vec_<E, 3>, E> &target) const {
    _camera.clip_near = close_bounded(
        distance(target.center, _camera.eye) - target.radius, 1e-2, 1e8);
    _camera.clip_far = close_bounded(
        distance(target.center, _camera.eye) + target.radius, 1e2, 1e8);
  }

  void translate(const vec_<E, 3> &t) const {
    _camera.eye += t;
    _camera.center += t;
  }

  void move_eye_around(const vec_<E, 3> &t, bool up_fixed = true) const {
    E dist = distance(_camera.eye, _camera.center);
    _camera.eye += t;
    _camera.eye =
        (_camera.eye - _camera.center).normalized() * dist + _camera.center;
    if (!up_fixed) {
      _camera.up = _camera.rightward().cross(_camera.forward());
    }
  }

private:
  perspective_camera<E> &_camera;
};

// manipulate
template <class E> inline camera_manip<E> manipulate(perspective_camera<E> &c) {
  return camera_manip<E>(c);
}
}
