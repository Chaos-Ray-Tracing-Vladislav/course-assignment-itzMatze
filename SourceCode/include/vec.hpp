#pragma once
#include <cstdint>
#include <cassert>
#include <cmath>
#include <ostream>
#include <iomanip>
#include <type_traits>

// chaos math
namespace cm {
template<typename T, int N>
struct Vec
{
  constexpr Vec() = default;
  constexpr explicit Vec(const T value)
  {
    for (uint32_t i = 0; i < N; i++) values[i] = value;
  }
  constexpr explicit Vec(const T vals[N])
  {
    for (uint32_t i = 0; i < N; i++) values[i] = vals[i];
  }
  constexpr Vec(const std::initializer_list<T>& vals)
  {
    assert(vals.size() == N);
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
  operator Vec<T2, N>() const
  {
    Vec<T2, N> result;
    for (uint32_t i = 0; i < N; i++) result[i] = T2(values[i]);
    return result;
  }

  T values[N];
};

// only the operators that are needed are implemented
// so, not every expected overload might be there already
template<typename T1, typename T2, int N>
Vec<T1, N> operator+(const Vec<T1, N>& a, const Vec<T2, N>& b)
{
  T1 vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = a.values[i] + T1(b.values[i]);
  return Vec<T1, N>(vals);
}

template<typename T1, typename T2, int N>
Vec<T1, N> operator+(const Vec<T1, N>& a, const T2 b)
{
  T1 vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = a.values[i] + T1(b);
  return Vec<T1, N>(vals);
}

template<typename T, int N>
Vec<T, N> operator-(const Vec<T, N>& a, const Vec<T, N>& b)
{
  return operator+(a, -b);
}

template<typename T1, typename T2, int N>
Vec<T1, N> operator-(const Vec<T1, N>& a, const T2 b)
{
  return operator+(a, -b);
}

template<typename T, int N>
Vec<T, N> operator-(const Vec<T, N>& a)
{
  T vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = -a.values[i];
  return Vec<T, N>(vals);
}

template<typename T1, typename T2, int N>
Vec<T1, N> operator/(const Vec<T1, N>& a, const Vec<T2, N>& b)
{
  T1 vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = a.values[i] / T1(b.values[i]);
  return Vec<T1, N>(vals);
}

template<typename T1, typename T2, int N>
Vec<T1, N> operator/(const Vec<T1, N>& a, const T2 b)
{
  T1 vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = a.values[i] / T1(b);
  return Vec<T1, N>(vals);
}

template<typename T1, typename T2, int N>
Vec<T1, N> operator*(const Vec<T1, N>& a, const T2 b)
{
  T1 vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = a.values[i] * T1(b);
  return Vec<T1, N>(vals);
}

template<typename T1, typename T2, int N>
Vec<T1, N> operator*(const T2 a, const Vec<T1, N>& b)
{
  return operator*(b, a);
}

template<typename T1, typename T2, int N>
Vec<T1, N> operator*(const Vec<T1, N>& a, const Vec<T2, N>& b)
{
  T1 vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = a.values[i] * T1(b.values[i]);
  return Vec<T1, N>(vals);
}

template<typename T, int N>
bool operator==(const Vec<T, N>& a, const Vec<T, N>& b)
{
  for (uint32_t i = 0; i < N; i++)
  {
    if (a[i] != b[i]) return false;
  }
  return true;
}

template<typename T, int N>
float dot(const Vec<T, N>& a, const Vec<T, N>& b)
{
  float result = 0.0;
  for (uint32_t i = 0; i < N; i++) result += a.values[i] * b.values[i];
  return result;
}

template<typename T, int N>
float length(const Vec<T, N>& a)
{
  float result = 0.0;
  for (uint32_t i = 0; i < N; i++) result += a.values[i] * a.values[i];
  return std::sqrt(result);
}

template<typename T, int N>
Vec<T, N> normalize(const Vec<T, N>& a) requires(std::is_floating_point<T>::value)
{
  return a / length(a);
}

template<typename T1, typename T2, int N>
Vec<T1, N> pow(const Vec<T1, N>& a, const T2 b)
{
  Vec<T1, N> result;
  for (uint32_t i = 0; i < N; i++) result[i] = std::pow(a[i], T1(b));
  return result;
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

template<typename T>
struct Vec<T, 2>
{
  constexpr Vec() = default;
  constexpr explicit Vec(const T value) : x(value), y(value) {}
  constexpr Vec(const T x, const T y) : x(x), y(y) {}
  constexpr Vec(const Vec& other) : x(other.x), y(other.y) {}
  constexpr Vec(const T vals[2]) : x(vals[0]), y(vals[1]) {}
  constexpr Vec(const std::initializer_list<T>& vals)
  {
    assert(vals.size() == 2);
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
  operator Vec<T2, 2>() const
  {
    Vec<T2, 2> result;
    for (uint32_t i = 0; i < 2; i++) result[i] = T2(values[i]);
    return result;
  }

  union {
    T values[2];
    struct {
      T x, y;
    };
    struct {
      T u, v;
    };
  };
};

template<typename T>
std::ostream& operator<<(std::ostream& out, const Vec<T, 2>& a)
{
  out << std::fixed << "(" << a.x << ", " << a.y << ")";
  return out;
}

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

using Vec2 = Vec<float, 2>;
using Vec3 = Vec<float, 3>;
using Vec2i = Vec<int32_t, 2>;
using Vec3i = Vec<int32_t, 3>;
using Vec2u = Vec<uint32_t, 2>;
using Vec3u = Vec<uint32_t, 3>;
} // namespace cm

