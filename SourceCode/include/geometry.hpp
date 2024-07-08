#pragma once
#include <vector>
#include "ray.hpp"
#include "object.hpp"
#include "material.hpp"
#include "interpolatable_data.hpp"
#include "hit_info.hpp"

class Geometry
{
public:
  Geometry() = default;
  Geometry(const InterpolatableData<Object>& objects, const std::vector<Material>& materials);
  uint32_t add_object(const Object& object);
  void add_material(const Material& material);
  InterpolatableData<Object>& get_interpolatable_objects();
  const InterpolatableData<Object>& get_interpolatable_objects() const;
  const std::vector<Object>& get_objects() const;
  const std::vector<Material>& get_materials() const;
  bool intersect(const Ray& ray, HitInfo& hit_info) const;

private:
  InterpolatableData<Object> objects;
  std::vector<Material> materials;
};

Geometry interpolate(const Geometry& a, const Geometry& b, float weight);
