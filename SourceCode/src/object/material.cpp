#include "object/material.hpp"
#include "util/vec.hpp"

Material::Material() : type(MaterialType::Diffuse), albedo(cm::Vec3(0.99, 0.01, 0.55)), smooth_shading(true)
{}

Material::Material(MaterialType type, const cm::Vec3& albedo, bool smooth_shading) : type(type), albedo(albedo), smooth_shading(smooth_shading)
{}

cm::Vec3 Material::eval(const HitInfo& hit_info, const cm::Vec3& incident_dir, const cm::Vec3& outgoing_dir) const
{
  if (is_delta()) return cm::Vec3(0.0, 0.0, 0.0);
  const float cos_theta = cm::dot(outgoing_dir, smooth_shading ? hit_info.normal : hit_info.geometric_normal);
  return albedo * std::max(0.0f, cos_theta);
}

bool Material::sample_dir(const HitInfo& hit_info, const cm::Vec3& incident_dir, cm::Vec3& dir, cm::Vec3& attenuation) const
{
  if (is_delta())
  {
    dir = cm::reflect(incident_dir, smooth_shading ? hit_info.normal : hit_info.geometric_normal);
    attenuation = albedo;
    return true;
  }
  return false;
}

bool Material::is_delta() const
{
  return (type == MaterialType::Reflective);
}
