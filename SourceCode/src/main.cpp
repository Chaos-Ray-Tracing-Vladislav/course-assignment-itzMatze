#include <string>
#include <vector>

#include "vec.hpp"
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
#if 1
  // render all scenes
  for (uint32_t i = 0; i < 6; i++)
  {
    std::string file("09/scene" + std::to_string(i) + ".crtscene");
    if (load_scene_file(file, scene_file)) return 1;
    renderer.init(scene_file.scene, scene_file.settings.resolution, file);
    renderer.render();
  }
#endif
#if 0
  renderer.init(create_pyramid_star_scene(), {1920, 1080}, "pyramid_star");
  renderer.render();
#endif
  return 0;
}

