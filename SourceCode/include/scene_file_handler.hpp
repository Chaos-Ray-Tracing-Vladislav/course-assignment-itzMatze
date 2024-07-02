#pragma once
#include "vec.hpp"
#include "scene.hpp"

struct Settings
{
  cm::Vec2u resolution;
};

struct SceneFile
{
  Scene scene;
  Settings settings;
};

int load_scene_file(const std::string& file_path, SceneFile& scene_file);

