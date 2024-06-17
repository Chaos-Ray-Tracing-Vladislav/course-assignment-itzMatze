#pragma once
#include <string>
#include "scene.hpp"
#include "color.hpp"
#include "vec.hpp"

class Renderer
{
public:
  Renderer() = default;
  void init(const Scene& scene, const cm::Vec2u resolution, const std::string& name);
  void render();

private:
  Scene scene;
  cm::Vec2u resolution;
  std::string output_name;
  bool use_jittering = false;

  std::vector<Color> render_frame();
  cm::Vec2 get_camera_coordinates(cm::Vec2u pixel);
};

