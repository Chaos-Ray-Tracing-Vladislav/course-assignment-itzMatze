#pragma once
#include "scene/scene.hpp"
#include "util/vec.hpp"

struct Settings
{
  cm::Vec2u resolution;
  uint32_t bucket_size;
};

struct SceneFile
{
  Scene scene;
  Settings settings;
};

int load_scene_file(const std::string& file_path, SceneFile& scene_file);

