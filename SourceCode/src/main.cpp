#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

#include "vec.hpp"
#include "mat.hpp"
#include "color.hpp"
#include "image_factory.hpp"
#include "scene_factory.hpp"
#include "renderer.hpp"

static constexpr cm::Vec2u resolution(1920, 1080);

int main(int argc, char** argv)
{
  std::vector<Color> pixels;
  Renderer renderer;
  renderer.init(hw06_task02(), resolution, "hw06_task02");
  renderer.render();
  cm::Mat3 mat({{1.0f, 2.0f, 3.0f}, {1.0f, 2.0f, 3.0f}, {1.0f, 2.0f, 3.0f}});
  cm::Vec3 angles = {90.0, 0.0, 0.0};
  cm::Mat3 rotation_mat = cm::rotate(angles);
  cm::Vec3 vec(0.0, 1.0, 0.0);
  cm::Mat3 result = mat * mat;
  std::cout << mat << "\n * \n" << mat << "\n = \n" << result << std::endl;
  std::cout << "rotate " << vec << " by 90 degrees around the x-axis: " << rotation_mat * vec << std::endl;
  return 0;
}

