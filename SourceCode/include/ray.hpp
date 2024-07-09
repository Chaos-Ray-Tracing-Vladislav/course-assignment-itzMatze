#pragma once
#include "vec.hpp"
#include <limits>

struct RayConfig
{
  float max_t = std::numeric_limits<float>::max();
  bool anyhit = false;
  bool backface_culling = true;
};

class Ray
{
public:
  Ray(const cm::Vec3& origin, const cm::Vec3& dir, const RayConfig config = RayConfig());
  cm::Vec3 at(float t) const;

  cm::Vec3 origin;
  cm::Vec3 dir;
  RayConfig config;
};

