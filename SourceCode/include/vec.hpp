#pragma once
#include <cstdint>
#include <cmath>
#include <type_traits>

// chaos math
namespace cm {
template<typename T, int N>
class Vec
{
public:
  Vec() = default;
  explicit Vec(const T value)
  {
    for (uint32_t i = 0; i < N; i++) values[i] = value;
  }
  Vec(const T vals[N])
  {
    for (uint32_t i = 0; i < N; i++) values[i] = vals[i];
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

template<typename T, int N>
Vec<T, N> operator-(const Vec<T, N>& a, const Vec<T, N>& b)
{
  T vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = a.values[i] - b.values[i];
  return Vec<T, N>(vals);
}

template<typename T1, typename T2, int N>
Vec<T1, N> operator/(const Vec<T1, N>& a, const T2 b)
{
  T1 vals[N];
  for (uint32_t i = 0; i < N; i++) vals[i] = a.values[i] / T1(b);
  return Vec<T1, N>(vals);
}

template<typename T>
struct Vec<T, 2>
{
  Vec() = default;
  Vec(const T value) : x(value), y(value) {}
  Vec(const T x, const T y) : x(x), y(y) {}
  Vec(const Vec& other) : x(other.x), y(other.y) {}
  Vec(const T vals[2]) : x(vals[0]), y(vals[1]) {}
  union {
    T values[2];
    struct {
      T x, y;
    };
  };
};

template<typename T>
struct Vec<T, 3>
{
  Vec() = default;
  Vec(const T value) : x(value), y(value), z(value) {}
  Vec(const T x, const T y, const T z) : x(x), y(y), z(z) {}
  Vec(const Vec& other) : x(other.x), y(other.y), z(other.z) {}
  Vec(const T vals[3]) : x(vals[0]), y(vals[1]), z(vals[2]) {}
  union {
    T values[3];
    struct {
      T x, y, z;
    };
  };
};

typedef Vec<float, 2> Vec2;
typedef Vec<float, 3> Vec3;
typedef Vec<int32_t, 2> Vec2i;
typedef Vec<int32_t, 3> Vec3i;
typedef Vec<uint32_t, 2> Vec2u;
typedef Vec<uint32_t, 3> Vec3u;
} // namespace cm

