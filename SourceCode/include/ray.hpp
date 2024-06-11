#pragma once
#include "vec.hpp"

class Ray
{
public:
  Ray() = default;
  Ray(const cm::Vec3& origin, const cm::Vec3& dir);
  cm::Vec3 at(float t) const;

  cm::Vec3 origin;
  cm::Vec3 dir;
};

