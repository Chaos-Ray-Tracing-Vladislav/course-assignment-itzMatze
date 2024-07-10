#pragma once
#include <string>
#include "renderer/color.hpp"
#include "scene/scene.hpp"
#include "util/vec.hpp"

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

  std::vector<Color> render_frame() const;
  cm::Vec2 get_camera_coordinates(cm::Vec2u pixel) const;
};

