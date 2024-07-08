#pragma once

#include "vec.hpp"
#include "hit_info.hpp"

enum class MaterialType
{
  Reflective,
  Diffuse
};

class Material
{
public:
  Material();
  Material(MaterialType type, const cm::Vec3& albedo, bool smooth_shading = true);
  cm::Vec3 eval(const HitInfo& hit_info, const cm::Vec3& incident_dir, const cm::Vec3& outgoing_dir) const;
  bool sample_dir(const HitInfo& hit_info, const cm::Vec3& incident_dir, cm::Vec3& dir, cm::Vec3& attenuation) const;
  // is material dirac delta reflective
  bool is_delta() const;

private:
  MaterialType type;
  cm::Vec3 albedo;
  bool smooth_shading;
};
