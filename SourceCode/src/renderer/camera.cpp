#include "renderer/camera.hpp"
#include "util/mat.hpp"
#include "util/spatial_configuration.hpp"
#include "util/vec.hpp"

CameraConfig::CameraConfig() :
  spatial_conf(cm::rotate(cm::Vec3(0.0, 0.0, 0.0)), cm::Vec3(0.0, 0.0, 0.0)),
  focal_length(0.03) /* 30mm camera lens */,
  sensor_size(0.036) /* standard full frame sensor width 36mm */
{}

SpatialConfiguration& CameraConfig::get_spatial_conf() { return spatial_conf; }

const SpatialConfiguration& CameraConfig::get_spatial_conf() const { return spatial_conf; }

float CameraConfig::get_focal_length() const { return focal_length; }

void CameraConfig::set_focal_length(float new_focal_length) { focal_length = new_focal_length; }

float CameraConfig::get_sensor_size() const { return sensor_size; }

void CameraConfig::set_sensor_size(float new_sensor_size) { sensor_size = new_sensor_size; }

CameraConfig interpolate(const CameraConfig& a, const CameraConfig& b, float weight)
{
  CameraConfig result;
  result.get_spatial_conf() = interpolate(a.get_spatial_conf(), b.get_spatial_conf(), weight);
  result.set_focal_length((1 - weight) * a.get_focal_length() + weight * b.get_focal_length());
  result.set_sensor_size((1 - weight) * a.get_sensor_size() + weight * b.get_sensor_size());
  return result;
}

Camera::Camera(const CameraConfig& config)
  : spatial_conf(config.get_spatial_conf()), sensor_size(config.get_sensor_size())
{
  // camera coordinate system
  upper_left_corner = spatial_conf.get_position() + (sensor_size / 2.0) * spatial_conf.get_y_axis() - (sensor_size / 2.0) * spatial_conf.get_x_axis() - config.get_focal_length() * spatial_conf.get_z_axis();
}

Ray Camera::get_ray(const cm::Vec2 pixel) const
{
  const cm::Vec3 pixel_pos = upper_left_corner + (pixel.x * sensor_size * spatial_conf.get_x_axis()) - (pixel.y * sensor_size * spatial_conf.get_y_axis());
  return Ray(spatial_conf.get_position(), pixel_pos - spatial_conf.get_position());
}
