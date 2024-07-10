#include "renderer/renderer.hpp"
#include "image/image_file_handler.hpp"
#include "renderer/color.hpp"
#include "util/random_generator.hpp"

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

std::vector<Color> Renderer::render_frame() const
{
  // store all rays that need to be traced, their accumulated attenuation, and their depth
  struct PathVertex
  {
    Ray ray;
    cm::Vec3 attenuation;
    uint32_t depth;
  };
  std::vector<PathVertex> path_vertices;

  std::vector<Color> pixels(resolution.x * resolution.y);
  HitInfo hit_info;
  for (uint32_t y = 0; y < resolution.y; y++)
  {
    for (uint32_t x = 0; x < resolution.x; x++)
    {
      path_vertices.clear();
      // add initial vertex of the camera
      path_vertices.push_back(PathVertex{scene.get_camera().get_ray(get_camera_coordinates({x, y})), cm::Vec3(1.0, 1.0, 1.0), 0});
      Color color(0.0, 0.0, 0.0);
      // trace rays as long as there are rays left to trace
      while (!path_vertices.empty())
      {
        PathVertex path_vertex = path_vertices.back();
        path_vertices.pop_back();
        if (scene.get_geometry().intersect(path_vertex.ray, hit_info))
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
              const float light_distance = cm::length(light.get_position() - hit_info.pos);
              // trace shadow ray with small offset in the direction of the normal to avoid shadow acne
              const Ray shadow_ray(hit_info.pos + RAY_START_OFFSET * hit_info.geometric_normal, outgoing_dir, RayConfig{.max_t = light_distance, .anyhit = true, .backface_culling = false});
              HitInfo shadow_hit_info;
              if (scene.get_geometry().intersect(shadow_ray, shadow_hit_info)) continue;
              const float light_surface = 4.0 * M_PI * light_distance * light_distance;
              cm::Vec3 contribution = cm::Vec3(light.get_intensity() / light_surface);
              contribution *= path_vertex.attenuation * material.eval(hit_info, path_vertex.ray.dir, outgoing_dir);
              color.value += contribution;
            }
          }
          std::vector<BSDFSample> bsdf_samples = material.get_bsdf_samples(hit_info, path_vertex.ray.dir);
          for (const auto& bsdf_sample : bsdf_samples)
          {
            const uint32_t depth = path_vertex.depth + 1;
            if (depth < 16)
            {
              const PathVertex next_path_vertex = PathVertex{bsdf_sample.ray, path_vertex.attenuation * bsdf_sample.attenuation, depth};
              path_vertices.push_back(next_path_vertex);
            }
          }
#endif
        }
        else
        {
          // background color
          color.value += scene.get_background_color().value * path_vertex.attenuation;
        }
      }
      pixels[y * resolution.x + x] = color;
    }
  }
  return pixels;
}

cm::Vec2 Renderer::get_camera_coordinates(cm::Vec2u pixel) const
{
  // offset to either get a random position inside of the pixel square or the center of the pixel
  cm::Vec2 offset = use_jittering ? cm::Vec2(rng::random_float(), rng::random_float()) : cm::Vec2(0.5);
  cm::Vec2 pixel_coordinates = (cm::Vec2(pixel) + offset) / cm::Vec2(resolution);
  float aspect_ratio = float(resolution.y) / float(resolution.x);
  pixel_coordinates.y *= aspect_ratio;
  pixel_coordinates.y += (1.0 - aspect_ratio) / 2.0;
  return pixel_coordinates;
}

