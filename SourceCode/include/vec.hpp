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

  T values[N];
};

// only the operators that are needed are implemented
// so, not every expected overload might be there already
template<typename T, int N>
Vec<T, N> operator+(const Vec<T, N>& a, const Vec<T, N>& b)
{
  T vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = a.values[i] + b.values[i];
  return Vec<T, N>(vals);
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

template<typename T, int N>
float dot(const Vec<T, N>& a, const cm::Vec<T, N>& b)
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

  union {
    T values[2];
    struct {
      T x, y;
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

