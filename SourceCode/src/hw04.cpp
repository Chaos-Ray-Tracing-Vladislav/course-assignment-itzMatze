#include <iostream>
#include "util/vec3.hpp"

int main(int argc, char** argv)
{
  std::cout << "\n==================== Task 2 ====================" << std::endl;
  {
    const cm::Vec3 a(3.5, 0.0, 0.0);
    const cm::Vec3 b(1.75, 3.5, 0.0);
    std::cout << "cross(" << a << ", " << b << ") = " << cm::cross(a, b) << std::endl;
  }
  {
    const cm::Vec3 a(3.0, -3.0, 1.0);
    const cm::Vec3 b(4.0, 9.0, 3.0);
    std::cout << "cross(" << a << ", " << b << ") = " << cm::cross(a, b) << std::endl;
  }
  // area of parallelogram is equal to magnitude of cross product vector
  {
    const cm::Vec3 a(3.0, -3.0, 1.0);
    const cm::Vec3 b(4.0, 9.0, 3.0);
    std::cout << "length(cross(" << a << ", " << b << ")) = " << cm::length(cm::cross(a, b)) << std::endl;
  }
  {
    const cm::Vec3 a(3.0, -3.0, 1.0);
    const cm::Vec3 b(-12.0, 12.0, -4.0);
    std::cout << "length(cross(" << a << ", " << b << ")) = " << cm::length(cm::cross(a, b)) << std::endl;
  }

  std::cout << "\n==================== Task 3 ====================" << std::endl;
  auto processTriangle = [](const cm::Vec3& a, const cm::Vec3& b, const cm::Vec3& c)
  {
    std::cout << "Triangle: " << a << ", " << b << ", " << c << std::endl;
    const cm::Vec3 ab(b - a);
    const cm::Vec3 ac(c - a);
    std::cout << "Normal: " << cm::normalize(cm::cross(cm::normalize(ab), cm::normalize(ac))) << std::endl;
    // area of a triangle is half the area of the corresponding parallelogram
    std::cout << "Area: " << cm::length(cm::cross(ab, ac)) / 2.0 << std::endl;
  };
  processTriangle({-1.75, -1.75, -3.0}, {1.75, -1.75, -3.0}, {0.0, 1.75, -3.0});
  std::cout << std::endl;
  processTriangle({0.0, 0.0, -1.0}, {1.0, 0.0, 1.0}, {-1.0, 0.0, 1.0});
  std::cout << std::endl;
  processTriangle({0.56, 1.11, 1.12}, {0.44, -2.368, -0.54}, {-1.56, 0.15, -1.92});
  return 0;
}

