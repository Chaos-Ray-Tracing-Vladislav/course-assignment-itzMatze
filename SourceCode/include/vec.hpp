#pragma once

class Vec3
{
public:
  Vec3() = default;
  explicit Vec3(float value);
  Vec3(float x, float y, float z);

  // only the operators that are needed are implemented
  // so, not every expected overload might be there already
  Vec3 operator+(const Vec3& other) const;
  Vec3 operator-(const Vec3& other) const;
  Vec3 operator/(const float other) const;

  float x;
  float y;
  float z;
};

