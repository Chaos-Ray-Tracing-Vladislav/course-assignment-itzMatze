#include "scene_file_handler.hpp"
#include <iostream>
#include <fstream>

#include "rapidjson/document.h"

#include "scene_builder.hpp"

int load_scene_file(const std::string& file_path, SceneFile& scene_file)
{
  const std::string path(std::string("../../Scenes/") + file_path);
  SceneBuilder scene_builder;
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open())
  {
    std::cerr << "Failed to open file: " << path << std::endl;
    return 1;
  };
  std::stringstream file_stream;
  file_stream << file.rdbuf();
  std::string file_content = file_stream.str();

  rapidjson::Document doc;
  doc.Parse(file_content.c_str());
  if (doc.HasParseError())
  {
    std::cerr << "Failed to parse file: " << path << std::endl;
    return 2;
  }

  const auto& rj_image_settings = doc["settings"]["image_settings"];

  scene_file.settings.resolution = cm::Vec2u(rj_image_settings["width"].GetUint(), rj_image_settings["height"].GetUint());

  const auto& rj_background_color = doc["settings"]["background_color"].GetArray();
  scene_builder.set_background(Color(rj_background_color[0].GetFloat(), rj_background_color[1].GetFloat(), rj_background_color[2].GetFloat()));

  const auto& rj_cam_matrix = doc["camera"]["matrix"];
  scene_builder.get_camera().set_up(cm::Vec3(rj_cam_matrix[3].GetFloat(), rj_cam_matrix[4].GetFloat(), rj_cam_matrix[5].GetFloat()));
  scene_builder.get_camera().set_view_dir(-cm::Vec3(rj_cam_matrix[6].GetFloat(), rj_cam_matrix[7].GetFloat(), rj_cam_matrix[8].GetFloat()));

  const auto& rj_position = doc["camera"]["position"];
  scene_builder.get_camera().set_origin(cm::Vec3(rj_position[0].GetFloat(), rj_position[1].GetFloat(), rj_position[2].GetFloat()));

  const auto& rj_objects = doc["objects"].GetArray();
  for (const auto& object : rj_objects)
  {
    const auto& rj_vertices = object["vertices"].GetArray();
    std::vector<cm::Vec3> vertices;
    for (size_t i = 0; i < rj_vertices.Size(); i += 3)
    {
      vertices.emplace_back(cm::Vec3(rj_vertices[i].GetFloat(), rj_vertices[i + 1].GetFloat(), rj_vertices[i + 2].GetFloat()));
    }

    const auto& rj_indices = object["triangles"].GetArray();
    std::vector<Triangle> triangles;
    for (size_t i = 0; i < rj_indices.Size(); i += 3)
    {
      triangles.emplace_back(vertices[rj_indices[i].GetInt()], vertices[rj_indices[i + 1].GetInt()], vertices[rj_indices[i + 2].GetInt()]);
    }
    scene_builder.get_geometry().add_new_object(triangles);
  }

  scene_builder.get_camera().set_focal_length(0.012);
  scene_file.scene = scene_builder.build_scene();
  return 0;
}

