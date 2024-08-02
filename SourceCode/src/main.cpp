#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "image/image_factory.hpp"
#include "renderer/color.hpp"
#include "renderer/renderer.hpp"
#include "scene/scene_factory.hpp"
#include "scene/scene_file_handler.hpp"
#include "util/timer.hpp"
#include "util/vec.hpp"

static constexpr cm::Vec2u resolution(1920, 1080);

int main(int argc, char** argv)
{
  std::vector<Color> pixels;
  SceneFile scene_file;
  Renderer renderer;
  const uint32_t thread_count = std::thread::hardware_concurrency();
  std::cerr << "Using " << thread_count << " threads" << std::endl;
#if 0
  // render all scenes
  for (uint32_t i = 0; i < 1; i++)
  {
    std::string file("13/scene" + std::to_string(i) + ".crtscene");
    Timer t;
    if (load_scene_file(file, scene_file) != 0) return 1;
    std::cout << "Scene \"" << file << "\" loaded: " << t.restart<std::milli>() << "ms" << std::endl;
    renderer.init(scene_file, file, thread_count);
    std::cout << "Renderer initialized: " << t.restart<std::milli>() << "ms" << std::endl;
    renderer.render();
    std::cout << "Rendering finished: " << t.restart<std::milli>() << "ms" << std::endl;
  }
#endif
#if 1
  scene_file.scene = create_pyramid_star_scene();
  scene_file.settings.resolution = cm::Vec2u(1920, 1080);
  scene_file.settings.bucket_size = 20;
  renderer.init(scene_file, "pyramid_star", thread_count);
  renderer.render();
#endif
  return 0;
}

