#pragma once

#include <vector>
#include "triangle.hpp"

class Object
{
public:
  Object() = default;
  Object(const std::vector<Triangle>& triangles);
  const std::vector<Triangle>& get_triangles() const;
  bool intersect(const Ray& ray, HitInfo& hit_info) const;

private:
  std::vector<Triangle> triangles;
};

Object interpolate(const Object& a, const Object& b, float weight);
