#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
#include <random>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "color.hpp"
#include "vec.hpp"

constexpr uint32_t image_width = 1920;
constexpr uint32_t image_height = 1080;

enum class FileType
{
  ppm,
  png
};

// pixel hex values are assumed to be in abgr format
void save_image(std::vector<Color> pixels, const std::string& name, int width, int height, FileType type = FileType::png)
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
    stbi_write_png(image_path.c_str(), width, height, 4, hex_pixels.data(), width * 4);
  }
  else if (type == FileType::ppm)
  {
    image_path.append(".ppm");
    std::ofstream file(image_path, std::ios::out | std::ios::binary);
    file << "P3\n" << width << " " << height << "\n" << 255 << "\n";
    for (uint32_t y = 0; y < height; y++)
    {
      for (uint32_t x = 0; x < width; x++)
      {
        // extract rgb values from hex representation and print to the file
        uint32_t pixel = pixels[y * width + x].get_hex_color();
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

Color get_random_color(float random)
{
  uint32_t rnd = random * 0x00ffffff;
  return Color(0xff000000 | rnd);
}

std::vector<Color> create_random_color_rectangles_image(const int width, const int height, const int rectangle_count_x = 4, const int rectangle_count_y = 4)
{
  std::vector<Color> pixels(width * height);
  std::vector<Color> rectangle_colors(rectangle_count_x * rectangle_count_y);
  std::mt19937 rnd(42);
  std::uniform_real_distribution<float> dis(0.0, 1.0);
  for (auto& color : rectangle_colors) color = get_random_color(dis(rnd));
  for (uint32_t y = 0; y < height; y++)
  {
    for (uint32_t x = 0; x < width; x++)
    {
      const uint32_t rectangle_idx_x = (x * rectangle_count_x) / width;
      const uint32_t rectangle_idx_y = (y * rectangle_count_y) / height;
      // get color of current rectangle, the color can be modulated by applying a random offset
      Color color(rectangle_colors[rectangle_idx_y * rectangle_count_x + rectangle_idx_x].value);
      pixels[y * width + x] = color;
    }
  }
  return pixels;
}

std::vector<Color> create_fix_color_rectangles_image(const int width, const int height, const int rectangle_count_x = 4, const int rectangle_count_y = 4)
{
  std::vector<Color> pixels(width * height);
  for (uint32_t y = 0; y < height; y++)
  {
    for (uint32_t x = 0; x < width; x++)
    {
      const uint32_t rectangle_idx_x = (x * rectangle_count_x) / width;
      const uint32_t rectangle_idx_y = (y * rectangle_count_y) / height;
      Color color;
      // define a repeating color pattern
      if (rectangle_idx_y & 1u)
      {
        if (rectangle_idx_x & 1u) color = Color(1.0, 0.0, 0.0);
        else color = Color(0.0, 1.0, 1.0);
      }
      else
      {
        if (rectangle_idx_x & 1u) color = Color(0.0, 0.0, 1.0);
        else color = Color(1.0, 0.0, 1.0);
      }
      pixels[y * width + x] = color;
    }
  }
  return pixels;
}

std::vector<Color> create_circle_image(const int width, const int height, const int radius = 200)
{
  std::vector<Color> pixels(width * height);
  for (uint32_t y = 0; y < height; y++)
  {
    for (uint32_t x = 0; x < width; x++)
    {
      const bool isInsideCircle = (std::pow(int(x) - width / 2, 2) + std::pow(int(y) - height / 2, 2) < (radius * radius));
      pixels[y * width + x] = isInsideCircle ? Color(0.0, 1.0, 1.0) : Color(0.0, 0.0, 0.0);
    }
  }
  return pixels;
}

int main(int argc, char** argv)
{
  std::vector<Color> rnd_rect_pixels = create_random_color_rectangles_image(image_width, image_height, 10, 10);
  std::vector<Color> fix_rect_pixels = create_fix_color_rectangles_image(image_width, image_height, 10, 10);
  std::vector<Color> circle_pixels = create_circle_image(image_width, image_height, 200);
  save_image(rnd_rect_pixels, "rnd_rect", image_width, image_height, FileType::png);
  save_image(fix_rect_pixels, "fix_rect", image_width, image_height, FileType::png);
  save_image(circle_pixels, "circle", image_width, image_height, FileType::png);
  return 0;
}

