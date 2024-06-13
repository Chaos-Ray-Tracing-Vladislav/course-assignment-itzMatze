#include "renderer.hpp"

void Renderer::init(const Scene& scene, const cm::Vec2u resolution, const CameraConfig& cam_config)
{
  this->scene = scene;
  cam = Camera(resolution, cam_config);
  this->resolution = resolution;
}

std::vector<Color> Renderer::trace()
{
  std::vector<Color> pixels(resolution.x * resolution.y);
  float t;
  cm::Vec3 p;
  for (uint32_t y = 0; y < resolution.y; y++)
  {
    for (uint32_t x = 0; x < resolution.x; x++)
    {
      if (scene.intersect(cam.get_ray({x, y}, false), t, p))
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

