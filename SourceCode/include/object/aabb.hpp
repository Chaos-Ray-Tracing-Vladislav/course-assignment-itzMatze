#pragma once

#include "renderer/ray.hpp"
#include "util/vec.hpp"

class AABB
{
public:
  AABB() = default;
  AABB(const cm::Vec3& min, const cm::Vec3& max);
  bool intersect(const Ray& ray) const;

  cm::Vec3 min;
  cm::Vec3 max;
};
