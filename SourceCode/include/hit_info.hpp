#pragma once

#include <limits>
#include "vec.hpp"

struct HitInfo
{
  cm::Vec3 pos = cm::Vec3(0.0);
  cm::Vec3 normal = cm::Vec3(0.0);
  cm::Vec2 bary = cm::Vec2(0.0);
  float t = std::numeric_limits<float>::max();
};
