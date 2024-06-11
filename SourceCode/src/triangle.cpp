#include "triangle.hpp"

Triangle::Triangle(const cm::Vec3& v0, const cm::Vec3& v1, const cm::Vec3& v2)
{
  vertices[0] = v0;
  vertices[1] = v1;
  vertices[2] = v2;
  normal = cm::normalize(cm::cross(cm::normalize(v1 - v0), cm::normalize(v2 - v0)));
}

bool Triangle::intersect(const Ray& ray, float& t, cm::Vec3& p) const
{
  const float dot_n_dir = cm::dot(ray.dir, normal);
  // backface culling
  if (dot_n_dir >= 0.0) return false;
  t = (cm::dot(normal, vertices[0]) - cm::dot(ray.origin, normal)) / dot_n_dir;
  // hit is behind the ray
  if (t < 0.0) return false;
  p = ray.at(t);
  // check if point lies inside or outside of the triangle
  {
    const cm::Vec3 e = cm::normalize(vertices[1] - vertices[0]);
    const cm::Vec3 vp = cm::normalize(p - vertices[0]);
    if (cm::dot(normal, cm::normalize(cm::cross(e, vp))) <= 0.0) return false;
  }
  {
    const cm::Vec3 e = cm::normalize(vertices[2] - vertices[1]);
    const cm::Vec3 vp = cm::normalize(p - vertices[1]);
    if (cm::dot(normal, cm::normalize(cm::cross(e, vp))) <= 0.0) return false;
  }
  {
    const cm::Vec3 e = cm::normalize(vertices[0] - vertices[2]);
    const cm::Vec3 vp = cm::normalize(p - vertices[2]);
    if (cm::dot(normal, cm::normalize(cm::cross(e, vp))) <= 0.0) return false;
  }
  return true;
}

