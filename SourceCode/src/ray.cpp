#include "ray.hpp"

Ray::Ray(const cm::Vec3& origin, const cm::Vec3& dir, float max_t, bool anyhit) : origin(origin), dir(cm::normalize(dir)), max_t(max_t), anyhit(anyhit)
{}

cm::Vec3 Ray::at(float t) const
{
  return origin + t * dir;
}

