#include "renderer.hpp"
#include "color.hpp"
#include "file_handler.hpp"
#include "random_generator.hpp"

void Renderer::init(const Scene& scene, const cm::Vec2u resolution, const std::string& name)
{
  this->scene = scene;
  this->resolution = resolution;
  output_name = name;
}

void Renderer::render()
{
  if (!scene.is_animated())
  {
    std::vector<Color> pixels = render_frame();
    save_single_image(pixels, output_name, resolution, FileType::png);
  }
  else
  {
    ImageSeries image_series(output_name, resolution, FileType::png);
    while (scene.step())
    {
      std::vector<Color> pixels = render_frame();
      image_series.save_image(pixels);
    }
  }
}

std::vector<Color> Renderer::render_frame()
{
  std::vector<Color> pixels(resolution.x * resolution.y);
  float t;
  cm::Vec3 p;
  for (uint32_t y = 0; y < resolution.y; y++)
  {
    for (uint32_t x = 0; x < resolution.x; x++)
    {
      if (scene.get_geometry().intersect(scene.get_camera().get_ray(get_camera_coordinates({x, y})), t, p))
      {
        // reduce color intensity with distance to visualize object distance in image
        float intensity = std::max(1.0 - (t / 10.0), 0.0);
        pixels[y * resolution.x + x] = Color(intensity, 0.0, intensity);
      }
      else
      {
        // background color
        pixels[y * resolution.x + x] = Color(0.0, 0.0, 0.2);
      }
    }
  }
  return pixels;
}

cm::Vec2 Renderer::get_camera_coordinates(cm::Vec2u pixel)
{
  // offset to either get a random position inside of the pixel square or the center of the pixel
  cm::Vec2 offset = use_jittering ? cm::Vec2(rng::random_float(), rng::random_float()) : cm::Vec2(0.5);
  cm::Vec2 pixel_coordinates = (cm::Vec2(pixel) + offset) / cm::Vec2(resolution);
  float aspect_ratio = float(resolution.y) / float(resolution.x);
  pixel_coordinates.y *= aspect_ratio;
  pixel_coordinates.y += (1.0 - aspect_ratio) / 2.0;
  return pixel_coordinates;
}

