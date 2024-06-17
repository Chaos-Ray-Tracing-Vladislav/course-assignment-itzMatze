#pragma once
#include <cstdint>
#include <memory>
#include <vector>
#include "geometry.hpp"
#include "camera.hpp"

class Scene
{
public:
  Scene() = default;
  Scene(const std::vector<std::shared_ptr<Geometry>>& geometry_keyframes, const std::vector<std::shared_ptr<CameraConfig>>& camera_keyframes, const std::vector<uint32_t>& frame_counts);
  const Geometry& get_geometry() const;
  const Camera& get_camera() const;
  bool step();
  bool is_animated();

private:
  std::vector<std::shared_ptr<Geometry>> geometry_keyframes;
  std::vector<std::shared_ptr<CameraConfig>> camera_keyframes;
  std::vector<uint32_t> frame_counts;
  Geometry current_geometry;
  Camera current_camera;
  uint32_t current_keyframe;
  uint32_t current_frame_step;
};

