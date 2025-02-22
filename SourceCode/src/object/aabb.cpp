#include "object/aabb.hpp"

AABB::AABB(const cm::Vec3& min, const cm::Vec3& max) : min(min), max(max)
{}

bool AABB::intersect(const Ray& ray) const
{
  // ray tracing gems II
  const cm::Vec3 t_lower = (min - ray.origin) * ray.get_inv_dir();
  const cm::Vec3 t_upper = (max - ray.origin) * ray.get_inv_dir();
  // the four t-intervals (for x-/y-/z-slabs, and ray p(t))
  const cm::Vec3 t_mins = cm::min(t_lower, t_upper);
  const cm::Vec3 t_maxes = cm::max(t_lower, t_upper);
  const float t_box_min = std::max(cm::max_component(t_mins), 0.0f);
  const float t_box_max = std::min(cm::min_component(t_maxes), ray.config.max_t);
  return t_box_min <= t_box_max;
}
