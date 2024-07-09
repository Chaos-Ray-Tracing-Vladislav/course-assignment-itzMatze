#include "ray.hpp"

Ray::Ray(const cm::Vec3& origin, const cm::Vec3& dir, const RayConfig config) : origin(origin), dir(cm::normalize(dir)), config(config)
{}

cm::Vec3 Ray::at(float t) const
{
  return origin + t * dir;
}
