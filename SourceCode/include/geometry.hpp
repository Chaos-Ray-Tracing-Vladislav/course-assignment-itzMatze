#pragma once
#include <vector>
#include "triangle.hpp"
#include "ray.hpp"
#include "object.hpp"
#include "interpolatable_data.hpp"
#include "hit_info.hpp"

class Geometry
{
public:
  Geometry() = default;
  Geometry(const std::vector<Triangle>& triangles);
  Geometry(const InterpolatableData<Object>& objects);
  void add_triangle(const Triangle& triangle);
  void add_object(const std::vector<Triangle>& triangles);
  const InterpolatableData<Object>& get_interpolatable_objects() const;
  const std::vector<Object>& get_objects() const;
  bool intersect(const Ray& ray, HitInfo& hit_info) const;

private:
  InterpolatableData<Object> objects;
};

Geometry interpolate(const Geometry& a, const Geometry& b, float weight);
