#include "file_handler.hpp"
#include <ctime>
#include <fstream>
#include <filesystem>
#include "stb_image_write.h"

void write_image(const std::vector<Color>& pixels, std::string& image_path, const cm::Vec2u resolution, FileType type)
{
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

void save_single_image(const std::vector<Color>& pixels, const std::string& name, const cm::Vec2u resolution, FileType type)
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
  write_image(pixels, image_path, resolution, type);
}

ImageSeries::ImageSeries(const std::string& directory, const cm::Vec2u resolution, FileType type) : dir(directory), resolution(resolution), type(type), image_idx(0)
{
  if (!std::filesystem::exists(directory)) std::filesystem::create_directory(directory);
}

void ImageSeries::save_image(const std::vector<Color>& pixels)
{
  std::string path = dir + std::to_string(image_idx++);
  write_image(pixels, path, resolution, type);
}

