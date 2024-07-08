#include "geometry.hpp"
#include <limits>
#include <vector>
#include <stdexcept>
#include "triangle.hpp"

Geometry::Geometry(const std::vector<Triangle>& triangles)
{
  for (const auto& triangle : triangles) add_triangle(triangle);
}

void Geometry::add_triangle(const Triangle& triangle)
{
  add_new_object({triangle});
}

void Geometry::add_new_object(const std::vector<Triangle>& triangles)
{
  // assign ascending indices to objects
  uint32_t id = objects.size() == 0 ? 0 : objects.back().get_id() + 1;
  objects.emplace_back(Object{triangles, id});
}

void Geometry::add_object_with_id(const Object& object)
{
  // make sure the ids are unique
#if !defined(NDEBUG)
  for (const auto& o : objects)
  {
    if (o.get_id() == object.get_id()) throw std::runtime_error("Error: Duplicate object id!");
  }
#endif
  objects.emplace_back(object);
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
    // test if object is intersected and if yes whether the intersection is closer than the previous ones
    if (object.intersect(ray, cur_t, cur_p) && (cur_t < t))
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
    if (objects_a[i].get_id() == objects_b[j].get_id())
    {
      result.add_object_with_id(interpolate(objects_a[i], objects_b[i], weight));
      i++;
      j++;
    }
    else if (objects_a[i].get_id() < objects_b[j].get_id())
    {
      // add objects from previous keyframe that are not present in the next keyframe
      result.add_object_with_id(objects_a[i]);
      i++;
    }
    // additional objects in the next keyframe will be added when that keyframe is reached
    else if (objects_a[i].get_id() > objects_b[j].get_id()) j++;
  }
  return result;
}

