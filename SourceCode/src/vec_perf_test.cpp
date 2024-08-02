#include <iostream>
#include <xmmintrin.h>
#include "util/timer.hpp"
#include "util/vec.hpp"
#include "util/vec3.hpp"

int main(int argc, char** argv)
{
  // use volatile variables to prevent the compiler from just removing everything because it is never used
  {
    Timer t;
    __m128 aSSE = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
    __m128 bSSE = _mm_set_ps(1.0f, 2.0f, 3.0f, 4.0f);
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      aSSE = _mm_add_ps(aSSE, bSSE);
    }
    std::cout << t.elapsed<std::milli>() << " ms" << std::endl;
    float result[4];
    _mm_storeu_ps(result, aSSE);
    volatile float compiler_keep_code[4] = {result[0], result[1], result[2], result[3]};
  }
  {
    Timer t;
    float a[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    float b[4] = {1.0f, 2.0f, 3.0f, 4.0f};
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      for (uint32_t i = 0; i < 4; i++) a[i] = a[i] + b[i];
    }
    std::cout << t.elapsed<std::milli>() << " ms" << std::endl;
    volatile float compiler_keep_code = a[0] + a[1] + a[2] + a[3];
  }
  std::cout << "\n==================== Vec 3 ====================" << std::endl;
  {
    std::cout << "Dot" << std::endl;
    Timer t;
    cm::Vec3 a(1.0f, 2.0f, 3.0f);
    cm::Vec3 b(4.0f, 5.0f, 6.0f);
    float result = 0.0;
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      result += cm::dot(a, b);
    }
    std::cout << t.elapsed<std::milli>() << " ms " << std::endl;
    volatile float compiler_keep_code = result;
  }
  {
    std::cout << "Min" << std::endl;
    Timer t;
    cm::Vec3 a(1.0f, 2.0f, 3.0f);
    cm::Vec3 b(4.0f, 5.0f, 6.0f);
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      a += cm::min(a, b);
    }
    std::cout << t.elapsed<std::milli>() << " ms " << std::endl;
    volatile float compiler_keep_code = a[0] + a[1] + a[2];
  }
  {
    std::cout << "Max" << std::endl;
    Timer t;
    cm::Vec3 a(1.0f, 2.0f, 3.0f);
    cm::Vec3 b(4.0f, 5.0f, 6.0f);
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      a += cm::max(a, b);
    }
    std::cout << t.elapsed<std::milli>() << " ms " << std::endl;
    volatile float compiler_keep_code = a[0] + a[1] + a[2];
  }
  {
    std::cout << "Max component" << std::endl;
    Timer t;
    cm::Vec3 a(1.0f, 2.0f, 3.0f);
    float result = 0.0;
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      result += cm::max_component(a);
    }
    std::cout << t.elapsed<std::milli>() << " ms " << std::endl;
    volatile float compiler_keep_code = result;
  }
  {
    std::cout << "Min component" << std::endl;
    Timer t;
    cm::Vec3 a(1.0f, 2.0f, 3.0f);
    float result = 0.0;
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      result += cm::min_component(a);
    }
    std::cout << t.elapsed<std::milli>() << " ms " << std::endl;
    volatile float compiler_keep_code = result;
  }
  {
    std::cout << "Addition" << std::endl;
    Timer t;
    cm::Vec3 a(3.5, 6.2, 4.1);
    cm::Vec3 b(1.7, 9.3, 7.9);
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      a = a + b;
    }
    std::cout << t.elapsed<std::milli>() << " ms" << std::endl;
    volatile float compiler_keep_code = a[0] + a[1] + a[2];
  }
  {
    std::cout << "Subtraction" << std::endl;
    Timer t;
    cm::Vec3 a(3.5, 6.2, 4.1);
    cm::Vec3 b(1.7, 9.3, 7.9);
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      a = a - b;
    }
    std::cout << t.elapsed<std::milli>() << " ms" << std::endl;
    volatile float compiler_keep_code = a[0] + a[1] + a[2];
  }
  {
    std::cout << "Multiplication" << std::endl;
    Timer t;
    cm::Vec3 a(3.5, 6.2, 4.1);
    cm::Vec3 b(1.7, 9.3, 7.9);
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      a = a * b;
    }
    std::cout << t.elapsed<std::milli>() << " ms" << std::endl;
    volatile float compiler_keep_code = a[0] + a[1] + a[2];
  }
  {
    std::cout << "Division" << std::endl;
    Timer t;
    cm::Vec3 a(3.5, 6.2, 4.1);
    cm::Vec3 b(1.7, 9.3, 7.9);
    for (uint32_t i = 0; i < 100'000'000; i++)
    {
      a = a / b;
    }
    std::cout << t.elapsed<std::milli>() << " ms" << std::endl;
    volatile float compiler_keep_code = a[0] + a[1] + a[2];
  }
  return 0;
}

