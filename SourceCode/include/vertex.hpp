#pragma once

#include "vec.hpp"

struct Vertex
{
  constexpr Vertex() : pos(0.0, 0.0, 0.0), normal(0.0, 0.0, 0.0)
  {}
  constexpr Vertex(const cm::Vec3& pos) : pos(pos), normal(0.0, 0.0, 0.0)
  {}
  constexpr Vertex(const cm::Vec3& pos, const cm::Vec3& normal) : pos(pos), normal(normal)
  {}
  cm::Vec3 pos;
  cm::Vec3 normal;
};
