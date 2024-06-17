#pragma once
#include <vector>
#include "triangle.hpp"
#include "ray.hpp"
#include "vec.hpp"

// currently an object consists only of a single triangle
struct Object
{
  Triangle triangle;
  uint32_t id;
};

class Geometry
{
public:
  Geometry() = default;
  Geometry(const std::vector<Triangle>& triangles);
  void add_triangle(const Triangle& triangle);
  void add_triangles(const std::vector<Triangle>& triangles);
  const std::vector<Object>& get_objects() const;
  bool intersect(const Ray& ray, float& t, cm::Vec3& p) const;

private:
  std::vector<Object> objects;
};

Geometry interpolate(const Geometry& a, const Geometry& b, float weight);

