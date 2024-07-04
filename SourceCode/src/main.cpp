#include <cstdint>
#include <string>
#include <vector>
#include <iostream>

#include "vec.hpp"
#include "mat.hpp"
#include "color.hpp"
#include "image_factory.hpp"
#include "scene_factory.hpp"
#include "scene_file_handler.hpp"
#include "renderer.hpp"

static constexpr cm::Vec2u resolution(1920, 1080);

int main(int argc, char** argv)
{
  std::vector<Color> pixels;
  SceneFile scene_file;
  Renderer renderer;
  // render all scenes
  for (uint32_t i = 0; i < 4; i++)
  {
    std::string file("08/scene" + std::to_string(i) + ".crtscene");
    if (load_scene_file(file, scene_file)) return 1;
    renderer.init(scene_file.scene, scene_file.settings.resolution, file);
    renderer.render();
  }
  return 0;
}

