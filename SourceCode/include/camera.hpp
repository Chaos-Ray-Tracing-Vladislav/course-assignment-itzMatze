#pragma once
#include "vec.hpp"
#include "ray.hpp"

struct CameraConfig
{
  const cm::Vec3 origin = cm::Vec3(0.0, 0.0, 0.0);
  const cm::Vec3 view_dir = cm::Vec3(0.0, 0.0, -1.0);
  const cm::Vec3 up = cm::Vec3(0.0, 1.0, 0.0);
  float focal_length = 0.03; // 30mm camera lens
  float sensor_width = 0.036; // standard full frame sensor width 36mm
};

/*
 * ray tracing camera that can generate rays for each pixel
 * resolution of the image that the rays are generated for
 * orientation and parameters of the camera
*/
class Camera
{
public:
  Camera() = default;
  Camera(const cm::Vec2u& resolution, const CameraConfig& config = CameraConfig());

  Ray get_ray(const cm::Vec2u pixel, bool use_jittering = false);

private:
  cm::Vec3 origin;
  // position of the camera sensor
  cm::Vec3 upper_left_corner;
  float pixel_size;
  // camera coordinate system
  cm::Vec3 right, up, backward;
};

