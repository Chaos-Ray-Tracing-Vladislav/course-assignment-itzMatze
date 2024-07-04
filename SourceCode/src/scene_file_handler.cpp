#include "scene_file_handler.hpp"
#include <iostream>
#include <fstream>

#include "rapidjson/document.h"

#include "scene_builder.hpp"

cm::Vec3 get_vec3(const rapidjson::GenericValue<rapidjson::UTF8<>>& rj_vec3, int base_index = 0)
{
  return cm::Vec3(rj_vec3[base_index].GetFloat(), rj_vec3[base_index + 1].GetFloat(), rj_vec3[base_index + 2].GetFloat());
}

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

  scene_builder.get_camera().set_origin(get_vec3(doc["camera"]["position"]));

  if (doc.HasMember("lights"))
  {
    const auto& rj_lights = doc["lights"].GetArray();
    for (const auto& light : rj_lights)
    {
      float intensity = light["intensity"].GetFloat();
      cm::Vec3 position = get_vec3(light["position"]);
      scene_builder.get_lights().add_new_data(Light(intensity, position));
    }
  }

  const auto& rj_objects = doc["objects"].GetArray();
  for (const auto& object : rj_objects)
  {
    const auto& rj_vertices = object["vertices"].GetArray();
    std::vector<cm::Vec3> vertices;
    for (size_t i = 0; i < rj_vertices.Size(); i += 3)
    {
      vertices.emplace_back(get_vec3(rj_vertices, i));
    }

    const auto& rj_indices = object["triangles"].GetArray();
    std::vector<Triangle> triangles;
    for (size_t i = 0; i < rj_indices.Size(); i += 3)
    {
      triangles.emplace_back(vertices[rj_indices[i].GetInt()], vertices[rj_indices[i + 1].GetInt()], vertices[rj_indices[i + 2].GetInt()]);
    }
    scene_builder.get_geometry().add_object(triangles);
  }

  scene_builder.get_camera().set_focal_length(0.012);
  scene_file.scene = scene_builder.build_scene();
  return 0;
}

