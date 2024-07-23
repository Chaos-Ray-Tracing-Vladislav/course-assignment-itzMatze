#pragma once
#include "vec.hpp"
#include <ostream>

// chaos math
namespace cm {
template<typename T>
struct Vec<T, 3>
{
  constexpr Vec() = default;
  constexpr explicit Vec(const T value) : x(value), y(value), z(value) {}
  constexpr Vec(const T x, const T y, const T z) : x(x), y(y), z(z) {}
  constexpr Vec(const Vec& other) : x(other.x), y(other.y), z(other.z) {}
  constexpr Vec(const T vals[3]) : x(vals[0]), y(vals[1]), z(vals[2]) {}
  constexpr Vec(const std::initializer_list<T>& vals)
  {
    assert(vals.size() == 3);
    uint32_t i = 0;
    for (const auto& val : vals)
    {
      values[i++] = val;
    }
  }

  template<typename T2>
  T& operator[](const T2 idx) requires(std::is_integral<T2>::value)
  {
    return values[idx];
  }

  template<typename T2>
  T operator[](const T2 idx) const requires(std::is_integral<T2>::value)
  {
    return values[idx];
  }

  template<typename T2>
  void operator*=(const T2 value)
  {
    *this = *this * value;
  }

  template<typename T2>
  void operator/=(const T2 value)
  {
    *this = *this / value;
  }

  template<typename T2>
  void operator+=(const T2 value)
  {
    *this = *this + value;
  }

  template<typename T2>
  operator Vec<T2, 3>() const
  {
    Vec<T2, 3> result;
    for (uint32_t i = 0; i < 3; i++) result[i] = T2(values[i]);
    return result;
  }

  union {
    T values[3];
    struct {
      T x, y, z;
    };
  };
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Vec<T, 3>& a)
{
  out << std::fixed << "(" << a.x << ", " << a.y << ", " << a.z << ")";
  return out;
}

template<typename T>
Vec<T, 3> cross(const Vec<T, 3>& a, const Vec<T, 3>& b) requires(std::is_floating_point<T>::value)
{
  Vec<T, 3> result;
  result.x = a.y * b.z - a.z * b.y;
  result.y = a.z * b.x - a.x * b.z;
  result.z = a.x * b.y - a.y * b.x;
  return result;
}

template<typename T>
Vec<T, 3> reflect(const Vec<T, 3>& incident_dir, const Vec<T, 3>& normal) requires(std::is_floating_point<T>::value)
{
  return incident_dir - 2.0 * cm::dot(incident_dir, normal) * normal;
}

template<typename T>
Vec<T, 3> refract(const Vec<T, 3>& incident_dir, const Vec<T, 3>& normal, T eta) requires(std::is_floating_point<T>::value)
{
  const T k = 1.0 - eta * eta * (1.0 - dot(incident_dir, normal) * dot(incident_dir, normal));
  if (k < 0.0) return Vec<T, 3>(0.0);
  else return eta * incident_dir - (eta * dot(incident_dir, normal) + std::sqrt(k)) * normal;
}

using Vec3 = Vec<float, 3>;
using Vec3i = Vec<int32_t, 3>;
using Vec3u = Vec<uint32_t, 3>;
} // namespace cm
