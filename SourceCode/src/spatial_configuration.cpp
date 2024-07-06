#include "spatial_configuration.hpp"
#include "quat.hpp"

SpatialConfiguration::SpatialConfiguration(const cm::Mat3& orientation, const cm::Vec3& position) : orientation(cm::quat_cast(orientation)), position(position)
{
  update_coordinate_system();
}

SpatialConfiguration::SpatialConfiguration(const cm::Quatf& orientation, const cm::Vec3& position) : orientation(orientation), position(position)
{
  update_coordinate_system();
}

void SpatialConfiguration::translate(const cm::Vec3& translation)
{
  position += translation;
}

void SpatialConfiguration::set_position(const cm::Vec3& position)
{
  this->position = position;
}

void SpatialConfiguration::rotate(cm::Vec3 angles, bool use_radian)
{
  rotate(angles.x, angles.y, angles.z, use_radian);
}

void SpatialConfiguration::rotate(float yaw, float pitch, float roll, bool use_radian)
{
  // calculate incremental change in angles with respect to camera coordinate system
  if (!use_radian)
  {
    yaw = yaw * (M_PI / 180.0);
    pitch = pitch * (M_PI / 180.0);
    roll = roll * (M_PI / 180.0);
  }
  cm::Quatf q_yaw = cm::angle_axis(yaw, local_y_axis);
  cm::Quatf q_pitch = cm::angle_axis(pitch, local_x_axis);
  cm::Quatf q_roll = cm::angle_axis(roll, local_z_axis);

  // apply incremental change to camera orientation
  orientation = cm::normalize(q_yaw * q_pitch * q_roll * orientation);
  update_coordinate_system();
}

void SpatialConfiguration::set_orientation(const cm::Mat3& orientation)
{
  this->orientation = cm::quat_cast(orientation);
  update_coordinate_system();
}

void SpatialConfiguration::set_orientation(const cm::Quatf& orientation)
{
  this->orientation = orientation;
  update_coordinate_system();
}

cm::Quatf SpatialConfiguration::get_orientation() const
{
  return orientation;
}

cm::Vec3 SpatialConfiguration::get_position() const
{
  return position;
}

cm::Vec3 SpatialConfiguration::get_x_axis() const
{
  return local_x_axis;
}

cm::Vec3 SpatialConfiguration::get_y_axis() const
{
  return local_y_axis;
}

cm::Vec3 SpatialConfiguration::get_z_axis() const
{
  return local_z_axis;
}

SpatialConfiguration interpolate(const SpatialConfiguration& a, const SpatialConfiguration& b, float weight)
{
  const cm::Quatf orientation = cm::slerp(a.get_orientation(), b.get_orientation(), weight);
  const cm::Vec3 position = (1 - weight) * a.get_position() + weight * b.get_position();
  return SpatialConfiguration(orientation, position);
}

void SpatialConfiguration::update_coordinate_system()
{
  const cm::Quatf q_x_axis = cm::normalize(orientation * cm::Quatf(0.0f, x_axis) * cm::conjugate(orientation));
  const cm::Quatf q_y_axis = cm::normalize(orientation * cm::Quatf(0.0f, y_axis) * cm::conjugate(orientation));
  const cm::Quatf q_z_axis = cm::normalize(orientation * cm::Quatf(0.0f, z_axis) * cm::conjugate(orientation));
  local_x_axis = cm::normalize(cm::Vec3(q_x_axis.x, q_x_axis.y, q_x_axis.z));
  local_y_axis = cm::normalize(cm::Vec3(q_y_axis.x, q_y_axis.y, q_y_axis.z));
  local_z_axis = cm::normalize(cm::Vec3(q_z_axis.x, q_z_axis.y, q_z_axis.z));
}
