#include "object/geometry.hpp"
#include <vector>
#include "util/interpolatable_data.hpp"

Geometry::Geometry(const InterpolatableData<Object>& objects, const std::vector<Material>& materials) : objects(objects), materials(materials)
{}

const InterpolatableData<Object>& Geometry::get_interpolatable_objects() const
{
  return objects;
}

const std::vector<Material>& Geometry::get_materials() const
{
  return materials;
}

bool Geometry::intersect(const Ray& ray, HitInfo& hit_info) const
{
  hit_info.t = ray.config.max_t;
  HitInfo cur_hit_info;
  for (const auto& object : objects.get_data())
  {
    // test if object is intersected and if yes whether the intersection is closer than the previous ones
    if (object.intersect(ray, cur_hit_info) && (cur_hit_info.t < hit_info.t))
    {
      hit_info = cur_hit_info;
      if (ray.config.anyhit) return true;
    }
  }
  // if an intersection was found, t is the distance to this intersection instead of maximum float value
  return (hit_info.t < ray.config.max_t);
}

Geometry interpolate(const Geometry& a, const Geometry& b, float weight)
{
  return Geometry(interpolate(a.get_interpolatable_objects(), b.get_interpolatable_objects(), weight), a.get_materials());
}
