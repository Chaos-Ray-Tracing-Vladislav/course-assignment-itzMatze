#include "object.hpp"
#include "triangle.hpp"

Object::Object(const std::vector<Triangle>& triangles) : triangles(triangles)
{}

const std::vector<Triangle>& Object::get_triangles() const
{
  return triangles;
}

bool Object::intersect(const Ray& ray, HitInfo& hit_info) const
{
  HitInfo cur_hit_info;
  for (const auto& triangle : triangles)
  {
    // test if object is intersected and if yes whether the intersection is closer than the previous ones
    if (triangle.intersect(ray, cur_hit_info) && (cur_hit_info.t < hit_info.t))
    {
      hit_info = cur_hit_info;
    }
  }
  // if an intersection was found, t is the distance to this intersection instead of maximum float value
  return (hit_info.t < std::numeric_limits<float>::max());
}

Object interpolate(const Object& a, const Object& b, float weight)
{
  std::vector<Triangle> triangles;
  const std::vector<Triangle>& triangles_a = a.get_triangles();
  const std::vector<Triangle>& triangles_b = b.get_triangles();
  assert(triangles_a.size() == triangles_b.size());
  for (uint32_t i = 0; i < triangles_a.size(); i++)
  {
      // interpolate triangle
      cm::Vec3 vertices[3];
      for (uint32_t k = 0; k < 3; k++) vertices[k] = (1.0 - weight) * triangles_a[i].vertices[k] + weight * triangles_b[i].vertices[k];
      triangles.emplace_back(Triangle(vertices[0], vertices[1], vertices[2]));
  }
  return Object(triangles);
}
