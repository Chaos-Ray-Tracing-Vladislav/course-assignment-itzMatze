#pragma once

#include <vector>
#include "vec.hpp"
#include "hit_info.hpp"
#include "ray.hpp"

enum class MaterialType
{
  Refractive,
  Reflective,
  Diffuse
};

struct MaterialParameters
{
  cm::Vec3 albedo = cm::Vec3(0.99, 0.01, 0.55);
  float ior = 1.5f;
  bool smooth_shading = true;
};

struct BSDFSample
{
  BSDFSample(const Ray& ray) : ray(ray), attenuation(1.0, 1.0, 1.0)
  {}
  BSDFSample(const Ray& ray, const cm::Vec3& attenuation) : ray(ray), attenuation(attenuation)
  {}
  Ray ray;
  cm::Vec3 attenuation;
};

class Material
{
public:
  Material();
  Material(MaterialType type, const MaterialParameters& params);
  cm::Vec3 eval(const HitInfo& hit_info, const cm::Vec3& incident_dir, const cm::Vec3& outgoing_dir) const;
  std::vector<BSDFSample> get_bsdf_samples(const HitInfo& hit_info, const cm::Vec3& incident_dir) const;
  // is material dirac delta reflective or refractive
  bool is_delta() const;

private:
  MaterialType type;
  MaterialParameters params;
};
