#pragma once
#include "vec.hpp"

class Triangle
{
  Triangle() = default;
  Triangle(const cm::Vec3& v0, const cm::Vec3& v1, const cm::Vec3& v2);

  cm::Vec3 vertices[3];
};

