#pragma once
#include "vec.hpp"
#include "ray.hpp"

class Camera
{
public:
  Camera() = default;
  Camera(const cm::Vec2u& resolution, const cm::Vec3& origin = cm::Vec3(0.0, 0.0, 0.0), const cm::Vec3& view_dir = cm::Vec3(0.0, 0.0, -1.0), const cm::Vec3& up = cm::Vec3(0.0, 1.0, 0.0), float sensor_width = 0.036 /* standard full frame sensor width 36mm */, float focal_length = 0.03 /* 30mm camera lens */);

  Ray get_ray(const cm::Vec2u& pixel, bool use_jittering = false);

private:
  cm::Vec3 origin;
  // position of the camera sensor
  cm::Vec3 upper_left_corner;
  float pixel_size;
  // camera coordinate system: right, up, backwards in camera perspective
  cm::Vec3 u, v, w;
};

