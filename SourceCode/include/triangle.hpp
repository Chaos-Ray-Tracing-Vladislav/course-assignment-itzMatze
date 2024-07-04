#pragma once
#include "ray.hpp"
#include "vec.hpp"
#include "hit_info.hpp"

class Triangle
{
public:
  Triangle() = default;
  Triangle(const cm::Vec3& v0, const cm::Vec3& v1, const cm::Vec3& v2);

  bool intersect(const Ray& ray, HitInfo& hit_info) const;

  cm::Vec3 vertices[3];
  cm::Vec3 geometric_normal;
};

