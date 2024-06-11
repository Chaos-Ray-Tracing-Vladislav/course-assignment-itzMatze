#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include "stb_image_write.h"

#include "vec.hpp"
#include "color.hpp"
#include "image_factory.hpp"

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
#if 0
  std::vector<Color> rnd_rect_pixels = create_random_color_rectangles_image(resolution, 10, 10);
  save_image(rnd_rect_pixels, "rnd_rect", resolution, FileType::png);
  std::vector<Color> fix_rect_pixels = create_fix_color_rectangles_image(resolution, 10, 10);
  save_image(fix_rect_pixels, "fix_rect", resolution, FileType::png);
  std::vector<Color> circle_pixels = create_circle_image(resolution, 200);
  save_image(circle_pixels, "circle", resolution, FileType::png);
#endif
  std::vector<Color> cam_ray_vis_pixels = create_camera_ray_vis_image(resolution);
  save_image(cam_ray_vis_pixels, "cam_ray_vis", resolution, FileType::png);
  return 0;
}

