#pragma once
#include <vector>
#include "triangle.hpp"
#include "ray.hpp"
#include "vec.hpp"
#include "object.hpp"

class Geometry
{
public:
  Geometry() = default;
  Geometry(const std::vector<Triangle>& triangles);
  void add_triangle(const Triangle& triangle);
  void add_new_object(const std::vector<Triangle>& triangles);
  void add_object_with_id(const Object& object);
  const std::vector<Object>& get_objects() const;
  bool intersect(const Ray& ray, float& t, cm::Vec3& p) const;

private:
  std::vector<Object> objects;
};

Geometry interpolate(const Geometry& a, const Geometry& b, float weight);

