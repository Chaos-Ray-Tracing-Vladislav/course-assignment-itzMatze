#pragma once
#include <cstdint>
#include "vec.hpp"

class Color
{
public:
  Color();
  explicit Color(uint32_t hex_color);
  explicit Color(Vec3 rgb);
  Color(float r, float g, float b);
  Color(float r, float g, float b, float a);
  uint32_t get_hex_color() const;

  Vec3 value;
  float alpha;
};

