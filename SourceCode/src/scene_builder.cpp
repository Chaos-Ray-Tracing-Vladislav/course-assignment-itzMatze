#include "scene_builder.hpp"
#include <vector>
#include "camera.hpp"

SceneBuilder::SceneBuilder()
{
  geometry_keyframes.push_back(std::make_shared<Geometry>());
  camera_keyframes.push_back(std::make_shared<CameraConfig>());
}

SceneBuilder::SceneBuilder(const std::vector<Triangle>& triangles, const CameraConfig& cam_config)
{
  geometry_keyframes.emplace_back(std::make_shared<Geometry>(triangles));
  camera_keyframes.emplace_back(std::make_shared<CameraConfig>(cam_config));
}

void SceneBuilder::new_keyframe(uint32_t frame_count)
{
  frame_counts.emplace_back(frame_count);
  // store next keyframe as pointer to last keyframe
  // copy keyframe in non-const getter because then the user wants to change it
  geometry_keyframes.emplace_back(geometry_keyframes.back());
  camera_keyframes.emplace_back(camera_keyframes.back());
}

const Geometry& SceneBuilder::get_geometry() const
{
  return *geometry_keyframes.back();
}

Geometry& SceneBuilder::get_geometry()
{
  if (geometry_keyframes.back().use_count() != 1)
  {
    // copy keyframe so it can be edited
    geometry_keyframes.back() = std::make_shared<Geometry>(*(geometry_keyframes.back()));
  }
  return *geometry_keyframes.back();
}

const CameraConfig& SceneBuilder::get_camera() const
{
  return *camera_keyframes.back();
}

CameraConfig& SceneBuilder::get_camera()
{
  if (camera_keyframes.back().use_count() != 1)
  {
    // copy keyframe so it can be edited
    camera_keyframes.back() = std::make_shared<CameraConfig>(*(camera_keyframes.back()));
  }
  return *camera_keyframes.back();
}

Scene SceneBuilder::build_scene()
{
    return Scene(geometry_keyframes, camera_keyframes, frame_counts);
}

