#pragma once

#include <string>
#include <vector>
#include "vec.hpp"
#include "color.hpp"

enum class FileType
{
  ppm,
  png
};

void save_single_image(const std::vector<Color>& pixels, const std::string& name, const cm::Vec2u resolution, FileType type = FileType::png);

class ImageSeries
{
public:
  ImageSeries(const std::string& directory, const cm::Vec2u resolution, FileType type = FileType::png);
  void save_image(const std::vector<Color>& pixels);
private:
  const std::string dir;
  const cm::Vec2u resolution;
  const FileType type;
  uint32_t image_idx;
};

