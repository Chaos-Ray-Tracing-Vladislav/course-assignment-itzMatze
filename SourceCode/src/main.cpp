#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include "stb_image_write.h"

#include "vec.hpp"
#include "color.hpp"
#include "image_factory.hpp"
#include "scene_factory.hpp"
#include "renderer.hpp"

static constexpr cm::Vec2u resolution(1920, 1080);

enum class FileType
{
  ppm,
  png
};

void save_image(const std::vector<Color>& pixels, const std::string& name, const cm::Vec2u resolution, FileType type = FileType::png)
{
  std::string image_path("../../Images/" + name);
  if (!name.empty())
  {
    image_path.append("_");
  }
  // add current time to filename
  std::time_t time = std::time(nullptr);
  std::tm local_time = *std::localtime(&time);
  char time_chars[20];
  std::strftime(time_chars, sizeof(time_chars), "%d-%m-%Y_%H-%M-%S", &local_time);
  std::string time_string(time_chars);
  image_path.append(time_string);
  if (type == FileType::png)
  {
    image_path.append(".png");
    std::vector<uint32_t> hex_pixels;
    for (const auto& color : pixels) hex_pixels.emplace_back(color.get_hex_color());
    stbi_write_png(image_path.c_str(), resolution.x, resolution.y, 4, hex_pixels.data(), resolution.x * 4);
  }
  else if (type == FileType::ppm)
  {
    image_path.append(".ppm");
    std::ofstream file(image_path, std::ios::out | std::ios::binary);
    file << "P3\n" << resolution.x << " " << resolution.y << "\n" << 255 << "\n";
    for (uint32_t y = 0; y < resolution.y; y++)
    {
      for (uint32_t x = 0; x < resolution.x; x++)
      {
        // extract rgb values from hex representation and print to the file
        uint32_t pixel = pixels[y * resolution.x + x].get_hex_color();
        file << (pixel & 0x000000ff) << " ";
        pixel >>= 8;
        file << (pixel & 0x000000ff) << " ";
        pixel >>= 8;
        file << (pixel & 0x000000ff) << "\t";
      }
      file << "\n";
    }
    file.close();
  }
}

int main(int argc, char** argv)
{
  std::vector<Color> pixels;
  Renderer renderer;
#if 0
  pixels = create_random_color_rectangles_image(resolution, 10, 10);
  save_image(pixels, "rnd_rect", resolution, FileType::png);
  pixels = create_fix_color_rectangles_image(resolution, 10, 10);
  save_image(pixels, "fix_rect", resolution, FileType::png);
  pixels = create_circle_image(resolution, 200);
  save_image(pixels, "circle", resolution, FileType::png);
  pixels = create_camera_ray_vis_image(resolution);
  save_image(pixels, "cam_ray_vis", resolution, FileType::png);
  renderer.init(create_single_triangle_scene(), resolution);
  pixels = renderer.trace();
  save_image(pixels, "single_triangle", resolution);
  renderer.init(create_triple_triangle_scene(), resolution);
  pixels = renderer.trace();
  save_image(pixels, "triple_triangle", resolution);
#endif
  renderer.init(create_pyramid_star_scene(), resolution);
  pixels = renderer.trace();
  save_image(pixels, "pyramid_star", resolution);
  return 0;
}

