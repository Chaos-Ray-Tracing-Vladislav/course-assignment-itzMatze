#include "triangle.hpp"

Triangle::Triangle(const cm::Vec3& v0, const cm::Vec3& v1, const cm::Vec3& v2)
{
  vertices[0] = v0;
  vertices[1] = v1;
  vertices[2] = v2;
}

