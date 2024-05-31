#include <cstdint>
#include <string>
#include <vector>
#include <ctime>
#include <fstream>
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

int main(int argc, char** argv)
{
  std::vector<Color> pixels(image_width * image_height, Color(0xffff00ff));
  save_image(pixels, "", image_width, image_height, FileType::png);
  return 0;
}

