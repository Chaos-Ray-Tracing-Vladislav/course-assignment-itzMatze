#include "renderer.hpp"
#include "color.hpp"
#include "image_file_handler.hpp"
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
  HitInfo hit_info;
  for (uint32_t y = 0; y < resolution.y; y++)
  {
    for (uint32_t x = 0; x < resolution.x; x++)
    {
      Ray ray = scene.get_camera().get_ray(get_camera_coordinates({x, y}));
      Color color(0.0, 0.0, 0.0);
      cm::Vec3 throughput(1.0, 1.0, 1.0);
      for (uint32_t i = 0; i < 16; i++)
      {
        if (scene.get_geometry().intersect(ray, hit_info))
        {
#if 0
          // barycentric coordinates debug visualization
          color = Color(hit_info.bary.u, hit_info.bary.v, 1.0);
          break;
#elif 0
          // normal debug visualization
          color = Color((hit_info.normal + 1.0) / 2.0);
          break;
#else
          Material material = (hit_info.material_idx == -1) ? Material() : scene.get_geometry().get_materials()[hit_info.material_idx];
          // if material is dirac delta reflective or there are no lights there is no need to evaluate lighting
          if (!material.is_delta() && scene.get_lights().size() > 0)
          {
            for (const auto& light : scene.get_lights())
            {
              const cm::Vec3 outgoing_dir = cm::normalize(light.get_position() - hit_info.pos);
              cm::Vec3 contribution = cm::Vec3(light.get_intensity());
              contribution *= throughput * material.eval(hit_info, ray.dir, outgoing_dir);
              contribution /= cm::length(light.get_position() - hit_info.pos);
              color.value += contribution;
            }
          }
          ray.origin = hit_info.pos;
          cm::Vec3 attenuation;
          if (!material.sample_dir(hit_info, ray.dir, ray.dir, attenuation)) break;
          throughput *= attenuation;
#endif
        }
        else
        {
          // background color
          color.value += scene.get_background_color().value * throughput;
          break;
        }
      }
      pixels[y * resolution.x + x] = color;
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

