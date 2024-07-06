#pragma once

#include "mat.hpp"
#include "vec.hpp"
#include "quat.hpp"

constexpr cm::Vec3 x_axis(1.0, 0.0, 0.0);
constexpr cm::Vec3 y_axis(0.0, 1.0, 0.0);
constexpr cm::Vec3 z_axis(0.0, 0.0, 1.0);

class SpatialConfiguration
{
public:
  SpatialConfiguration(const cm::Mat3& orientation, const cm::Vec3& position);
  SpatialConfiguration(const cm::Quat<float>& orientation, const cm::Vec3& position);
  void translate(const cm::Vec3& translation);
  void set_position(const cm::Vec3& position);
  void rotate(cm::Vec3 angles, bool use_radian = false);
  void rotate(float yaw, float pitch, float roll, bool use_radian = false);
  void set_orientation(const cm::Mat3& orientation);
  void set_orientation(const cm::Quat<float>& orientation);
  cm::Quat<float> get_orientation() const;
  cm::Vec3 get_position() const;
  cm::Vec3 get_x_axis() const;
  cm::Vec3 get_y_axis() const;
  cm::Vec3 get_z_axis() const;

private:
  cm::Quat<float> orientation;
  cm::Vec3 position;
  // coordinate system
  cm::Vec3 local_x_axis, local_y_axis, local_z_axis;

  void update_coordinate_system();
};

SpatialConfiguration interpolate(const SpatialConfiguration& a, const SpatialConfiguration& b, float weight);
