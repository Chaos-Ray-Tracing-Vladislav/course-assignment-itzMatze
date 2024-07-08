#pragma once

#include <vector>
#include "triangle.hpp"

class Object
{
public:
  Object() = default;
  Object(const std::vector<Triangle>& triangles, uint32_t id);
  uint32_t get_id() const;
  const std::vector<Triangle>& get_triangles() const;
  bool intersect(const Ray& ray, float& t, cm::Vec3& p) const;

private:
  std::vector<Triangle> triangles;
  uint32_t id;
};

Object interpolate(const Object& a, const Object& b, float weight);
