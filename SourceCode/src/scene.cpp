#include "scene.hpp"
#include <limits>
#include <vector>
#include "triangle.hpp"

Scene::Scene(const std::vector<Triangle>& triangles) : triangles(triangles) {}

void Scene::add_triangle(const Triangle& triangle)
{
  triangles.emplace_back(triangle);
}

const std::vector<Triangle>& Scene::get_triangles() const
{
  return triangles;
}

bool Scene::intersect(const Ray& ray, float& t, cm::Vec3& p) const
{
  t = std::numeric_limits<float>::max();
  float cur_t;
  cm::Vec3 cur_p;
  for (const auto& triangle : triangles)
  {
    // test if triangle is intersected and if yes whether the intersection is closer than the previous ones
    if (triangle.intersect(ray, cur_t, cur_p) && (cur_t < t))
    {
      t = cur_t;
      p = cur_p;
    }
  }
  // if an intersection was found, t is the distance to this intersection instead of maximum float value
  return (t < std::numeric_limits<float>::max());
}

