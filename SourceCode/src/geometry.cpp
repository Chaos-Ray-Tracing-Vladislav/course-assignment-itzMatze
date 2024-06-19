#include "geometry.hpp"
#include <limits>
#include <vector>
#include "triangle.hpp"

Geometry::Geometry(const std::vector<Triangle>& triangles)
{
  for (const auto& triangle : triangles) add_triangle(triangle);
}

void Geometry::add_triangle(const Triangle& triangle)
{
  // assign ascending indices to objects
  uint32_t id = objects.size() == 0 ? 0 : objects.back().id++;
  objects.emplace_back(Object{triangle, id});
}

void Geometry::add_triangles(const std::vector<Triangle>& new_triangles)
{
  for (const auto& triangle : new_triangles) add_triangle(triangle);
}

const std::vector<Object>& Geometry::get_objects() const
{
  return objects;
}

bool Geometry::intersect(const Ray& ray, float& t, cm::Vec3& p) const
{
  t = std::numeric_limits<float>::max();
  float cur_t;
  cm::Vec3 cur_p;
  for (const auto& object : objects)
  {
    // test if triangle is intersected and if yes whether the intersection is closer than the previous ones
    if (object.triangle.intersect(ray, cur_t, cur_p) && (cur_t < t))
    {
      t = cur_t;
      p = cur_p;
    }
  }
  // if an intersection was found, t is the distance to this intersection instead of maximum float value
  return (t < std::numeric_limits<float>::max());
}

Geometry interpolate(const Geometry& a, const Geometry& b, float weight)
{
  Geometry result;
  const std::vector<Object>& objects_a = a.get_objects();
  const std::vector<Object>& objects_b = b.get_objects();
  for (uint32_t i = 0, j = 0; i < objects_a.size() && j < objects_b.size();)
  {
    if (objects_a[i].id == objects_b[j].id)
    {
      // interpolate triangle
      cm::Vec3 vertices[3];
      for (uint32_t k = 0; k < 3; k++) vertices[k] = (1.0 - weight) * objects_a[i].triangle.vertices[k] + weight * objects_b[j].triangle.vertices[k];
      result.add_triangle(Triangle(vertices[0], vertices[1], vertices[2]));
      i++;
      j++;
    }
    else if (objects_a[i].id < objects_b[j].id)
    {
      // add objects from previous keyframe that are not present in the next keyframe
      result.add_triangle(objects_a[i].triangle);
      i++;
    }
    // additional objects in the next keyframe will be added when that keyframe is reached
    else if (objects_a[i].id > objects_b[j].id) j++;
  }
  return result;
}

