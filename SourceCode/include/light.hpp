#pragma once

#include "vec.hpp"

class Light
{
public:
  Light(float intensity, const cm::Vec3& position);
  cm::Vec3 get_contribution(const cm::Vec3& pos, const cm::Vec3& normal);
  float get_intensity() const;
  cm::Vec3 get_position() const;

private:
  float intensity;
  cm::Vec3 position;
};

Light interpolate(const Light& a, const Light& b, float weight);
