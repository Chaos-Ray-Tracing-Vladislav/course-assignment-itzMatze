#pragma once
#include <vector>
#include "triangle.hpp"
#include "ray.hpp"
#include "vec.hpp"

class Scene
{
public:
  Scene() = default;
  Scene(const std::vector<Triangle>& triangles);
  void add_triangle(const Triangle& triangle);
  const std::vector<Triangle>& get_triangles() const;
  bool intersect(const Ray& ray, float& t, cm::Vec3& p) const;

private:
  std::vector<Triangle> triangles;
};

