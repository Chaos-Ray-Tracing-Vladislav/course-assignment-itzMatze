#include "image_factory.hpp"
#include <cmath>
#include "camera.hpp"

std::vector<Color> create_random_color_rectangles_image(const cm::Vec2u resolution, const uint32_t rectangle_count_x, const uint32_t rectangle_count_y)
{
  std::vector<Color> pixels(resolution.x * resolution.y);
  std::vector<Color> rectangle_colors(rectangle_count_x * rectangle_count_y);
  for (auto& color : rectangle_colors) color = Color::get_random_color();
  for (uint32_t y = 0; y < resolution.y; y++)
  {
    for (uint32_t x = 0; x < resolution.x; x++)
    {
      const uint32_t rectangle_idx_x = (x * rectangle_count_x) / resolution.x;
      const uint32_t rectangle_idx_y = (y * rectangle_count_y) / resolution.y;
      // get color of current rectangle, the color can be modulated by applying a random offset
      Color color(rectangle_colors[rectangle_idx_y * rectangle_count_x + rectangle_idx_x].value);
      pixels[y * resolution.x + x] = color;
    }
  }
  return pixels;
}

std::vector<Color> create_fix_color_rectangles_image(const cm::Vec2u resolution, const uint32_t rectangle_count_x, const uint32_t rectangle_count_y)
{
  std::vector<Color> pixels(resolution.x * resolution.y);
  for (uint32_t y = 0; y < resolution.y; y++)
  {
    for (uint32_t x = 0; x < resolution.x; x++)
    {
      const uint32_t rectangle_idx_x = (x * rectangle_count_x) / resolution.x;
      const uint32_t rectangle_idx_y = (y * rectangle_count_y) / resolution.y;
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
      pixels[y * resolution.x + x] = color;
    }
  }
  return pixels;
}

std::vector<Color> create_circle_image(const cm::Vec2u resolution, const uint32_t radius)
{
  std::vector<Color> pixels(resolution.x * resolution.y);
  for (uint32_t y = 0; y < resolution.y; y++)
  {
    for (uint32_t x = 0; x < resolution.x; x++)
    {
      const bool isInsideCircle = (std::pow(int(x) - int(resolution.x) / 2, 2) + std::pow(int(y) - int(resolution.y) / 2, 2) < (radius * radius));
      pixels[y * resolution.x + x] = isInsideCircle ? Color(0.0, 1.0, 1.0) : Color(0.0, 0.0, 0.0);
    }
  }
  return pixels;
}

std::vector<Color> create_camera_ray_vis_image(const cm::Vec2u resolution)
{
  std::vector<Color> pixels(resolution.x * resolution.y);
  Camera cam(resolution);
  for (uint32_t y = 0; y < resolution.y; y++)
  {
    for (uint32_t x = 0; x < resolution.x; x++)
    {
      pixels[y * resolution.x + x] = Color((cam.get_ray({x, y}).dir + 1.0) / 2.0);
    }
  }
  return pixels;
}

