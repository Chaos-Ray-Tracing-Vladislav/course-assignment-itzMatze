#pragma once
#include "scene.hpp"
#include "camera.hpp"
#include "color.hpp"

class Renderer
{
public:
  Renderer() = default;
  void init(const Scene& scene, const cm::Vec2u resolution, const CameraConfig& cam_config = CameraConfig());
  std::vector<Color> trace();

private:
  Scene scene;
  Camera cam;
  cm::Vec2u resolution;
};

