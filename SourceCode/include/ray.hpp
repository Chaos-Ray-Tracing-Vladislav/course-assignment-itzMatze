#pragma once
#include "vec.hpp"
#include <limits>

class Ray
{
public:
  Ray() = default;
  Ray(const cm::Vec3& origin, const cm::Vec3& dir, float max_t = std::numeric_limits<float>::max(), bool anyhit = false);
  cm::Vec3 at(float t) const;

  cm::Vec3 origin;
  cm::Vec3 dir;
  float max_t;
  bool anyhit;
};

