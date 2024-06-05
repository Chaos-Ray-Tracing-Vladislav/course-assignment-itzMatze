#include "ray.hpp"

Ray::Ray(const cm::Vec3& origin, const cm::Vec3& dir) : origin(origin), dir(cm::normalize(dir)) {}

cm::Vec3 Ray::get_pos(float t)
{
  return origin + t * dir;
}

