#include "camera.hpp"

#include "vec.hpp"

CameraConfig::CameraConfig() :
  origin(cm::Vec3(0.0, 0.0, 0.0)),
  view_dir(cm::Vec3(0.0, 0.0, -1.0)),
  up(cm::Vec3(0.0, 1.0, 0.0)),
  focal_length(0.03) /* 30mm camera lens */,
  sensor_size(0.036) /* standard full frame sensor width 36mm */
{}

CameraConfig::CameraConfig(const cm::Vec3& origin, const cm::Vec3& view_dir, const cm::Vec3& up, float focal_length, float sensor_size) :
  origin(origin),
  view_dir(cm::normalize(view_dir)),
  up(cm::normalize(up)),
  focal_length(focal_length),
  sensor_size(sensor_size)
{}

cm::Vec3 CameraConfig::get_origin() const { return origin; }

void CameraConfig::set_origin(const cm::Vec3& new_origin) { origin = new_origin; }

cm::Vec3 CameraConfig::get_view_dir() const { return view_dir; }

void CameraConfig::set_view_dir(const cm::Vec3& new_view_dir) { view_dir = cm::normalize(new_view_dir); }

cm::Vec3 CameraConfig::get_up() const { return up; }

void CameraConfig::set_up(const cm::Vec3& new_up) { up = cm::normalize(new_up); }

float CameraConfig::get_focal_length() const { return focal_length; }

void CameraConfig::set_focal_length(float new_focal_length) { focal_length = new_focal_length; }

float CameraConfig::get_sensor_size() const { return sensor_size; }

void CameraConfig::set_sensor_size(float new_sensor_size) { sensor_size = new_sensor_size; }

CameraConfig interpolate(const CameraConfig& a, const CameraConfig& b, float weight)
{
  CameraConfig result;
  result.set_origin((1 - weight) * a.get_origin() + weight * b.get_origin());
  result.set_view_dir((1 - weight) * a.get_view_dir() + weight * b.get_view_dir());
  result.set_up((1 - weight) * a.get_up() + weight * b.get_up());
  result.set_focal_length((1 - weight) * a.get_focal_length() + weight * b.get_focal_length());
  result.set_sensor_size((1 - weight) * a.get_sensor_size() + weight * b.get_sensor_size());
  return result;
}

Camera::Camera(const CameraConfig& config)
  : origin(config.get_origin()), backward(-config.get_view_dir()), sensor_size(config.get_sensor_size())
{
  // camera coordinate system
  right = cm::normalize(cm::cross(config.get_up(), backward));
  up = cm::cross(backward, right);
  upper_left_corner = origin + (sensor_size / 2.0) * up - (sensor_size / 2.0) * right - config.get_focal_length() * backward;
}

Ray Camera::get_ray(const cm::Vec2 pixel) const
{
  const cm::Vec3 pixel_pos = upper_left_corner + (pixel.x * sensor_size * right) - (pixel.y * sensor_size * up);
  return Ray(origin, pixel_pos - origin);
}

