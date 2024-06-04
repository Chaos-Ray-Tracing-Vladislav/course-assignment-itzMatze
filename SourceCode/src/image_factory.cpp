#include "image_factory.hpp"
#include <cmath>

std::vector<Color> create_random_color_rectangles_image(const uint32_t width, const uint32_t height, const uint32_t rectangle_count_x, const uint32_t rectangle_count_y)
{
  std::vector<Color> pixels(width * height);
  std::vector<Color> rectangle_colors(rectangle_count_x * rectangle_count_y);
  for (auto& color : rectangle_colors) color = Color::get_random_color();
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

std::vector<Color> create_fix_color_rectangles_image(const uint32_t width, const uint32_t height, const uint32_t rectangle_count_x, const uint32_t rectangle_count_y)
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

// convert width and height to int as negative numbers are needed to compute circle
std::vector<Color> create_circle_image(const int width, const int height, const uint32_t radius)
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
