#pragma once
#include <memory>
#include <vector>
#include "geometry.hpp"
#include "camera.hpp"
#include "scene.hpp"

class SceneBuilder
{
public:
  SceneBuilder();
  SceneBuilder(const std::vector<Triangle>& triangles, const CameraConfig& cam_config = CameraConfig());

  void new_keyframe(uint32_t frame_count /* number of frames between previous and new keyframe */);

  const Geometry& get_geometry() const;
  Geometry& get_geometry();

  const CameraConfig& get_camera() const;
  CameraConfig& get_camera();

  Scene build_scene();

private:
  std::vector<std::shared_ptr<Geometry>> geometry_keyframes;
  std::vector<std::shared_ptr<CameraConfig>> camera_keyframes;
  std::vector<uint32_t> frame_counts;
};

