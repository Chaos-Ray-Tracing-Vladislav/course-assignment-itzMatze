#include "triangle.hpp"

#define EPSILON 0.0001

Triangle::Triangle(const cm::Vec3& v0, const cm::Vec3& v1, const cm::Vec3& v2)
{
  vertices[0] = v0;
  vertices[1] = v1;
  vertices[2] = v2;
  normal = cm::normalize(cm::cross(cm::normalize(v1 - v0), cm::normalize(v2 - v0)));
}

bool Triangle::intersect(const Ray& ray, HitInfo& hit_info) const
{
  const float dot_n_dir = cm::dot(ray.dir, normal);
  // backface culling
  if (dot_n_dir >= 0.0) return false;
  hit_info.t = (cm::dot(normal, vertices[0]) - cm::dot(ray.origin, normal)) / dot_n_dir;
  // hit is behind the ray
  if (hit_info.t < 0.0) return false;
  hit_info.pos = ray.at(hit_info.t);
  // check if point lies inside or outside of the triangle
  {
    const cm::Vec3 e = cm::normalize(vertices[1] - vertices[0]);
    const cm::Vec3 vp = cm::normalize(hit_info.pos - vertices[0]);
    if (cm::dot(normal, cm::cross(e, vp)) < -EPSILON) return false;
  }
  {
    const cm::Vec3 e = cm::normalize(vertices[2] - vertices[1]);
    const cm::Vec3 vp = cm::normalize(hit_info.pos - vertices[1]);
    if (cm::dot(normal, cm::cross(e, vp)) < -EPSILON) return false;
  }
  {
    const cm::Vec3 e = cm::normalize(vertices[0] - vertices[2]);
    const cm::Vec3 vp = cm::normalize(hit_info.pos - vertices[2]);
    if (cm::dot(normal, cm::cross(e, vp)) < -EPSILON) return false;
  }
  hit_info.normal = normal;
  return true;
}

