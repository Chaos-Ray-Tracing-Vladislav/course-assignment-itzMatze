#pragma once
#include <string>
#include "renderer/color.hpp"
#include "scene/scene.hpp"
#include "scene/scene_file_handler.hpp"
#include "util/vec2.hpp"

class Renderer
{
public:
  Renderer() = default;
  void init(const SceneFile& scene_file, const std::string& name, uint32_t thread_count = 1);
  void render();

private:
  struct ImageBucket
  {
    cm::Vec2u min;
    cm::Vec2u max;
  };

  std::shared_ptr<Scene> scene;
  cm::Vec2u resolution;
  std::string output_name;
  bool use_jittering = false;
  std::vector<ImageBucket> buckets;
  uint32_t thread_count;

  std::vector<Color> render_frame() const;
  void render_buckets(std::vector<Color>* pixels, std::atomic<uint32_t>* bucket_idx) const;
  cm::Vec2 get_camera_coordinates(cm::Vec2u pixel) const;
};

