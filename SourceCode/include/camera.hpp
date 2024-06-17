#pragma once
#include "vec.hpp"
#include "ray.hpp"

class CameraConfig
{
public:
  CameraConfig();
  CameraConfig(const cm::Vec3& origin, const cm::Vec3& view_dir, const cm::Vec3& up, float focal_length, float sensor_size);

  cm::Vec3 get_origin() const;
  void set_origin(const cm::Vec3& new_origin);
  cm::Vec3 get_view_dir() const;
  void set_view_dir(const cm::Vec3& new_view_dir);
  cm::Vec3 get_up() const;
  void set_up(const cm::Vec3& new_up);
  float get_focal_length() const;
  void set_focal_length(float new_focal_length);
  float get_sensor_size() const;
  void set_sensor_size(float new_sensor_size);

private:
  cm::Vec3 origin;
  cm::Vec3 view_dir;
  cm::Vec3 up;
  float focal_length;
  // sensor is quadratic
  float sensor_size;
};

CameraConfig interpolate(const CameraConfig& a, const CameraConfig& b, float weight);

/*
 * ray tracing camera that can generate rays for each pixel
 * resolution of the image that the rays are generated for
 * orientation and parameters of the camera
*/
class Camera
{
public:
  Camera(const CameraConfig& config = CameraConfig());

  // camera acts as a quadratic texture that can be accessed texture coordinates
  // sensor is defined in [0,1], but accesses outside are still valid
  Ray get_ray(const cm::Vec2 pixel) const;

private:
  cm::Vec3 origin;
  // position of the camera sensor
  cm::Vec3 upper_left_corner;
  // camera coordinate system
  cm::Vec3 right, up, backward;
  // sensor is quadratic
  float sensor_size;
};

