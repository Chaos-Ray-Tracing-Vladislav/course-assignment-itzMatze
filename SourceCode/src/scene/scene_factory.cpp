#include "scene/scene_factory.hpp"
#include "image/image_file_handler.hpp"
#include "object/material.hpp"
#include "object/object.hpp"
#include "scene/scene.hpp"
#include "scene/scene_builder.hpp"
#include "scene/scene_file_handler.hpp"
#include "util/quat.hpp"
#include "util/vec.hpp"

Scene create_single_triangle_scene()
{
  SceneBuilder scene_builder;
  const std::vector<Vertex> vertices{
    cm::Vec3(-1.0, -1.0, -5.0),
    cm::Vec3(1.0, -1.0, -5.0),
    cm::Vec3(0.0, 1.0, -5.0)
  };
  scene_builder.get_geometry().add_object(Object(vertices, {0, 1, 2}, SpatialConfiguration(), -1, true));
  return scene_builder.build_scene();
}

Scene create_triple_triangle_scene()
{
  SceneBuilder scene_builder;
  const std::vector<Vertex> vertices{
    cm::Vec3(-1.0, -1.0, -5.0),
    cm::Vec3(1.0, -1.0, -5.0),
    cm::Vec3(0.0, 1.0, -5.0),
    cm::Vec3(-2.0, -1.0, -4.0),
    cm::Vec3(0.0, -1.0, -4.0),
    cm::Vec3(-1.0, 1.0, -4.0),
    cm::Vec3(0.0, -1.0, -4.0),
    cm::Vec3(2.0, -1.0, -4.0),
    cm::Vec3(1.0, 1.0, -4.0)
  };
  scene_builder.get_geometry().add_object(Object(vertices, {0, 1, 2, 3, 4, 5, 6, 7, 8}, SpatialConfiguration(), -1, true));
  return scene_builder.build_scene();
}

Object add_star(const cm::Vec3& center, float inner_radius, float tip_length, uint32_t tip_count) {
  std::vector<Vertex> vertices{cm::Vec3(0.0, 0.0, 0.0)};
  std::vector<uint32_t> indices;
  // the angle that one tip takes up
  const float tip_angle = (2 * M_PI / float(tip_count));
  for (uint32_t i = 0; i < tip_count; i++)
  {
    const float angle0 = i * tip_angle;
    const float angle1 = angle0 - 0.5 * tip_angle;
    const float angle2 = angle0 + 0.5 * tip_angle;

    vertices.emplace_back(cm::Vec3(sin(angle0) * tip_length, cos(angle0) * tip_length, 0.0));
    vertices.emplace_back(cm::Vec3(sin(angle1) * inner_radius, cos(angle1) * inner_radius, 0.0));
    vertices.emplace_back(cm::Vec3(sin(angle2) * inner_radius, cos(angle2) * inner_radius, 0.0));

    indices.emplace_back(0);
    indices.emplace_back(vertices.size() - 3);
    indices.emplace_back(vertices.size() - 2);
    indices.emplace_back(0);
    indices.emplace_back(vertices.size() - 1);
    indices.emplace_back(vertices.size() - 3);
  }
  SpatialConfiguration spatial_conf;
  spatial_conf.set_position(center);
  return Object(vertices, indices, spatial_conf, -1, true);
}

Scene create_pyramid_star_scene()
{
  SceneBuilder scene_builder;
  std::vector<uint32_t> object_ids;
  // pyramid
  {
    const std::vector<Vertex> vertices{
      cm::Vec3(-1.0, -1.0, 1.0),
      cm::Vec3(1.0, -1.0, 1.0),
      cm::Vec3(1.0, -1.0, -1.0),
      cm::Vec3(-1.0, -1.0, -1.0),
      cm::Vec3(0.0, 1.0, 0.0)
    };
    const std::vector<uint32_t> indices{
      0, 1, 4,
      1, 2, 4,
      2, 3, 4,
      3, 0, 4,
      0, 2, 1,
      0, 3, 2
    };
    SpatialConfiguration spatial_conf;
    spatial_conf.rotate(45.0, 0.0, 0.0);
    // left pyramid
    spatial_conf.set_position(cm::Vec3(-2.0, 0.0, -6.0));
    object_ids.emplace_back(scene_builder.get_geometry().add_object(Object(vertices, indices, spatial_conf, -1, true)));
    // right pyramid
    spatial_conf.set_position(cm::Vec3(2.0, 0.0, -6.0));
    object_ids.emplace_back(scene_builder.get_geometry().add_object(Object(vertices, indices, spatial_conf, -1, true)));
    Object object;
    if (load_object_file("dragon.crtobject", object) != 0) return Scene();
    object.get_spatial_conf().rotate(-90.0, 0.0, 0.0);
    object.get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, -20.0));
    object_ids.emplace_back(scene_builder.get_geometry().add_object(object));
  }

  // star
  scene_builder.get_geometry().add_object(add_star(cm::Vec3(0.0, 0.5, -4.0), 0.1, 0.5, 5));

  {
    scene_builder.new_keyframe(30);
    for (uint32_t id : object_ids)
    {
      scene_builder.get_geometry().get_object(id).get_spatial_conf().rotate(90.0, 0.0, 0.0);
    }
    scene_builder.get_camera().get_spatial_conf().set_position(cm::Vec3(-4.0, 0.0, 1.0));
    const cm::Vec3 view_dir = cm::Vec3(0.0, 0.0, -6.0) - scene_builder.get_camera().get_spatial_conf().get_position();
    scene_builder.get_camera().get_spatial_conf().set_orientation(cm::quat_look_at(cm::normalize(view_dir)));
  }

  {
    scene_builder.new_keyframe(30);
    for (uint32_t id : object_ids)
    {
      scene_builder.get_geometry().get_object(id).get_spatial_conf().rotate(90.0, 0.0, 0.0);
    }
    scene_builder.get_camera().get_spatial_conf().set_position(cm::Vec3(-4.0, 4.0, 1.0));
  }

  {
    scene_builder.new_keyframe(30);
    for (uint32_t id : object_ids)
    {
      scene_builder.get_geometry().get_object(id).get_spatial_conf().rotate(90.0, 0.0, 0.0);
    }
    const cm::Vec3 view_dir = cm::Vec3(0.0, 0.0, -6.0) - scene_builder.get_camera().get_spatial_conf().get_position();
    scene_builder.get_camera().get_spatial_conf().set_orientation(cm::quat_look_at(cm::normalize(view_dir)));
  }

  {
    scene_builder.new_keyframe(30);
    for (uint32_t id : object_ids)
    {
      scene_builder.get_geometry().get_object(id).get_spatial_conf().rotate(90.0, 0.0, 0.0);
    }
    scene_builder.get_camera().get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, 0.0));
    scene_builder.get_camera().get_spatial_conf().set_orientation(cm::quat_look_at(cm::Vec3(0.0, 0.0, -1.0)));
  }

  {
    scene_builder.new_keyframe(15);
    for (uint32_t id : object_ids)
    {
      scene_builder.get_geometry().get_object(id).get_spatial_conf().rotate(45.0, 0.0, 0.0);
    }
    scene_builder.get_camera().get_spatial_conf().rotate(30.0, 0.0, 0.0);
  }
  {
    scene_builder.new_keyframe(30);
    for (uint32_t id : object_ids)
    {
      scene_builder.get_geometry().get_object(id).get_spatial_conf().rotate(90.0, 0.0, 0.0);
    }
    scene_builder.get_camera().get_spatial_conf().rotate(-60.0, 0.0, 0.0);
  }
  {
    scene_builder.new_keyframe(15);
    for (uint32_t id : object_ids)
    {
      scene_builder.get_geometry().get_object(id).get_spatial_conf().rotate(45.0, 0.0, 0.0);
    }
    scene_builder.get_camera().get_spatial_conf().rotate(30.0, 0.0, 0.0);
  }

  return scene_builder.build_scene();
}

Scene hw06_task02()
{
  SceneBuilder scene_builder;
  const std::vector<Vertex> vertices{
    cm::Vec3(-1.75, -1.75, -3.0),
    cm::Vec3(1.75, -1.75, -3.0),
    cm::Vec3(0.0, 1.75, -3.0)
  };
  scene_builder.get_geometry().add_object(Object(vertices, {0, 1, 2}, SpatialConfiguration(), -1, true));
  scene_builder.get_camera().get_spatial_conf().set_position(cm::Vec3(-4.0, 0.0, 1.0));
  const cm::Vec3 view_dir = cm::Vec3(0.0, 0.0, -3.0) - scene_builder.get_camera().get_spatial_conf().get_position();
  scene_builder.get_camera().get_spatial_conf().set_orientation(cm::quat_look_at(cm::normalize(view_dir)));
  return scene_builder.build_scene();
}

void remove_objects(SceneBuilder& scene_builder, std::vector<uint32_t>& object_ids)
{
  for (uint32_t id : object_ids)
  {
    scene_builder.get_geometry().remove_object(id);
  }
  object_ids.clear();
}

Scene create_progression_video_scene()
{
  SceneBuilder scene_builder;
  scene_builder.set_background(Color(cm::Vec3(0.0, 0.0, 0.2)));
  // textures
  const std::shared_ptr<Texture> white_texture = std::make_shared<Texture>(cm::Vec3(1.0, 1.0, 1.0));
  const std::shared_ptr<Texture> light_grey_texture = std::make_shared<Texture>(cm::Vec3(0.8, 0.8, 0.8));
  const std::shared_ptr<Texture> black_texture = std::make_shared<Texture>(cm::Vec3(0.0, 0.0, 0.0));
  const std::shared_ptr<Texture> pink_texture = std::make_shared<Texture>(cm::Vec3(0.99, 0.01, 0.55));
  const std::shared_ptr<Texture> cyan_texture = std::make_shared<Texture>(cm::Vec3(0.01, 0.99, 0.99));
  const std::shared_ptr<Texture> green_texture = std::make_shared<Texture>(cm::Vec3(0.01, 0.99, 0.01));
  const std::shared_ptr<Texture> red_texture = std::make_shared<Texture>(cm::Vec3(0.99, 0.01, 0.01));
  const std::shared_ptr<Texture> fire_texture = std::make_shared<Texture>(cm::Vec3(0.99, 0.4, 0.1));
  Texture::BitmapParameters bitmap_params;
  load_image("../../Scenes/dragon.jpg", bitmap_params.bitmap, bitmap_params.resolution);
  const std::shared_ptr<Texture> dragon_texture = std::make_shared<Texture>(bitmap_params);
  Texture::CheckerParameters checker_params{pink_texture, cyan_texture, 0.1};
  const std::shared_ptr<Texture> checker_pink_cyan_texture = std::make_shared<Texture>(checker_params);
  checker_params.even = white_texture;
  checker_params.odd = black_texture;
  checker_params.tile_size = 0.02;
  const std::shared_ptr<Texture> checker_black_white_texture = std::make_shared<Texture>(checker_params);
  checker_params.even = pink_texture;
  checker_params.odd = checker_black_white_texture;
  checker_params.tile_size = 0.1;
  const std::shared_ptr<Texture> checker_pink_checker_black_white_texture = std::make_shared<Texture>(checker_params);
  Texture::EdgesParameters edges_params{cyan_texture, pink_texture, 0.1};
  const std::shared_ptr<Texture> edges_pink_cyan_texture = std::make_shared<Texture>(edges_params);
  edges_params.edge = pink_texture;
  edges_params.center = checker_black_white_texture;
  const std::shared_ptr<Texture> edges_pink_checker_black_white_texture = std::make_shared<Texture>(edges_params);
  checker_params.even = fire_texture;
  checker_params.odd = dragon_texture;
  checker_params.tile_size = 0.2;
  const std::shared_ptr<Texture> checker_fire_dragon_texture = std::make_shared<Texture>(checker_params);
  // materials, the enum must be in the same order as the materials are added
  // the scene_builder api is not ideal to hardcode a scene in code
  // it should be used to load scenes and animations from a file
  enum MaterialIndex
  {
    MAT_DEBUG_ALBEDO = 0,
    MAT_DEBUG_BARY = 1,
    MAT_DEBUG_GEOM_NORMAL = 2,
    MAT_DEBUG_NORMAL = 3,
    MAT_DEBUG_TEX_COORDS = 4,
    MAT_DIFFUSE_WHITE = 5,
    MAT_DIFFUSE_GREEN = 6,
    MAT_DIFFUSE_RED = 7,
    MAT_MIRROR = 8,
    MAT_GLASS = 9,
    MAT_PINK_NON_SMOOTH = 10,
    MAT_CHECKER_PINK_CYAN = 11,
    MAT_CHECKER_PINK_CHECKER_BLACK_WHITE = 12,
    MAT_EDGES_PINK_CYAN = 13,
    MAT_EDGES_PINK_CHECKER_BLACK_WHITE = 14,
    MAT_DRAGON = 15,
    MAT_CHECKER_FIRE_DRAGON = 16
  };
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.show_albedo = true}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.show_bary = true}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.smooth_shading = false, .show_normal = true}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.show_normal = true}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.show_tex_coords = true}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.albedo_texture = white_texture}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.albedo_texture = green_texture}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.albedo_texture = red_texture}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Reflective, MaterialParameters{.albedo_texture = light_grey_texture, .smooth_shading = false}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Refractive, MaterialParameters{.albedo_texture = light_grey_texture}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.smooth_shading = false}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.albedo_texture = checker_pink_cyan_texture}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.albedo_texture = checker_pink_checker_black_white_texture}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.albedo_texture = edges_pink_cyan_texture}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.albedo_texture = edges_pink_checker_black_white_texture}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.albedo_texture = dragon_texture}));
  scene_builder.get_geometry().add_material(Material(MaterialType::Diffuse, MaterialParameters{.albedo_texture = checker_fire_dragon_texture}));
  // objects, preload used objects to only load one instance of each
  Object sphere_object;
  if (load_object_file("sphere.crtobject", sphere_object) != 0) return Scene();
  Object plane_object;
  if (load_object_file("plane.crtobject", plane_object) != 0) return Scene();
  Object dragon_object;
  if (load_object_file("dragon.crtobject", dragon_object) != 0) return Scene();
  dragon_object.get_spatial_conf().rotate(-90.0, 0.0, 0.0);
  // alternate object ids between current shown progression and next one
  // the pattern is to create the next scene, transition in some way from the current to the next scene, and then remove the previous scene
  std::vector<uint32_t> object_ids_a;
  std::vector<uint32_t> object_ids_b;
  std::vector<uint32_t> light_ids;
// progression 0, fix triangle
  {
    const std::vector<Vertex> vertices{
      cm::Vec3(-1.0, -1.0, -5.0),
      cm::Vec3(1.0, -1.0, -5.0),
      cm::Vec3(0.0, 1.0, -5.0)
    };
    object_ids_a.push_back(scene_builder.get_geometry().add_object(Object(vertices, {0, 1, 2}, SpatialConfiguration(), MAT_DEBUG_ALBEDO, true)));
    scene_builder.get_camera().get_spatial_conf().set_position(cm::Vec3(-4.0, 0.0, 0.0));
    cm::Vec3 view_dir = cm::Vec3(0.0, 0.0, -3.0) - scene_builder.get_camera().get_spatial_conf().get_position();
    scene_builder.get_camera().get_spatial_conf().set_orientation(cm::quat_look_at(cm::normalize(view_dir)));
    scene_builder.new_keyframe(60);
    scene_builder.get_camera().get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, 0.0));
    scene_builder.get_camera().get_spatial_conf().set_orientation(cm::quat_look_at(cm::Vec3(0.0, 0.0, -1.0)));
  }
// progression 1, rotating triangle
  {
    const std::vector<Vertex> vertices{
      cm::Vec3(-1.0, -1.0, -5.0),
      cm::Vec3(1.0, -1.0, -5.0),
      cm::Vec3(0.0, 1.0, -5.0)
    };
    object_ids_b.push_back(scene_builder.get_geometry().add_object(Object(vertices, {0, 1, 2}, SpatialConfiguration(cm::Vec3(0.0, 0.0, -100.0)), MAT_DEBUG_ALBEDO, true)));
  }
  scene_builder.new_keyframe(60);
  // move away progression 0
  for (uint32_t id : object_ids_a)
  {
    scene_builder.get_geometry().get_object(id).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 40.0));
  }
  // move in progression 1
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, -3.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().rotate(0.0, 0.0, 180.0);
  scene_builder.new_keyframe(60);
  remove_objects(scene_builder, object_ids_a);
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, -2.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().rotate(0.0, 0.0, 180);
// progression 2, normal debug vis
  {
    // create progression 2
    object_ids_a.emplace_back(scene_builder.get_geometry().add_object(sphere_object));
    scene_builder.get_geometry().get_object(object_ids_a.at(0)).material_idx = MAT_DEBUG_GEOM_NORMAL;
    scene_builder.get_geometry().get_object(object_ids_a.at(0)).get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, -99.0));
    object_ids_a.emplace_back(scene_builder.get_geometry().add_object(sphere_object));
    scene_builder.get_geometry().get_object(object_ids_a.at(1)).material_idx = MAT_DEBUG_NORMAL;
    scene_builder.get_geometry().get_object(object_ids_a.at(1)).get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, -101.0));
  }
  // move away progression 1
  for (uint32_t i = 0; i < 4; i++)
  {
    scene_builder.new_keyframe(4);
    scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2.0));
    scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().rotate(0.0, 0.0, 180);
    scene_builder.get_geometry().get_object(object_ids_a.at(0)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2.0));
    scene_builder.get_geometry().get_object(object_ids_a.at(1)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2.0));
  }
  scene_builder.new_keyframe(60);
  remove_objects(scene_builder, object_ids_b);
  // move in progression 2
  scene_builder.get_geometry().get_object(object_ids_a.at(0)).get_spatial_conf().set_position(cm::Vec3(3.0, 0.0, -10.0));
  scene_builder.get_geometry().get_object(object_ids_a.at(1)).get_spatial_conf().set_position(cm::Vec3(-3.0, 0.0, -10.0));
  scene_builder.new_keyframe(60);
  scene_builder.get_geometry().get_object(object_ids_a.at(0)).get_spatial_conf().rotate(180.0, 0.0, 0.0);
  scene_builder.get_geometry().get_object(object_ids_a.at(1)).get_spatial_conf().rotate(180.0, 0.0, 0.0);
  scene_builder.new_keyframe(60);
  scene_builder.get_geometry().get_object(object_ids_a.at(0)).get_spatial_conf().rotate(180.0, 0.0, 0.0);
  scene_builder.get_geometry().get_object(object_ids_a.at(1)).get_spatial_conf().rotate(180.0, 0.0, 0.0);
// progression 3, light demonstration
  {
    // create progression 3
    object_ids_b.emplace_back(scene_builder.get_geometry().add_object(plane_object));
    scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().set_position(cm::Vec3(-40.0, -8.0, 0.0));
    scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().set_scale(20.0);
    scene_builder.get_geometry().get_object(object_ids_b.at(0)).material_idx = MAT_DIFFUSE_WHITE;
    object_ids_b.emplace_back(scene_builder.get_geometry().add_object(sphere_object));
    scene_builder.get_geometry().get_object(object_ids_b.at(1)).get_spatial_conf().set_position(cm::Vec3(-40.0, 0.0, 0.0));
    scene_builder.get_geometry().get_object(object_ids_b.at(1)).material_idx = MAT_PINK_NON_SMOOTH;
    light_ids.emplace_back(scene_builder.get_lights().add_new_data(Light(4000.0, cm::Vec3(-30.0, 10.0, -10.0))));
    light_ids.emplace_back(scene_builder.get_lights().add_new_data(Light(1000.0, cm::Vec3(-40.0, 10.0, 0.0))));
    light_ids.emplace_back(scene_builder.get_lights().add_new_data(Light(400.0, cm::Vec3(-50.0, 10.0, 10.0))));
  }
  scene_builder.new_keyframe(60);
  scene_builder.get_geometry().get_object(object_ids_a.at(0)).get_spatial_conf().rotate(180.0, 0.0, 0.0);
  scene_builder.get_geometry().get_object(object_ids_a.at(1)).get_spatial_conf().rotate(180.0, 0.0, 0.0);
  // rotate camera to show progression 3
  scene_builder.get_camera().get_spatial_conf().set_orientation(cm::quat_look_at(cm::Vec3(-1.0, 0.0, 0.0)));
  // move the lights in a square and modulate their brightness
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-30.0, 10.0, 10.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(-50.0, 10.0, -10.0));
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(400.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(4000.0);
  scene_builder.new_keyframe(60);
  remove_objects(scene_builder, object_ids_a);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-50.0, 10.0, 10.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(-30.0, 10.0, -10.0));
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(4000.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(400.0);
  scene_builder.new_keyframe(60);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-50.0, 10.0, -10.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(-30.0, 10.0, 10.0));
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(400.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(4000.0);
  scene_builder.new_keyframe(60);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-30.0, 10.0, -10.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(-50.0, 10.0, 10.0));
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(4000.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(400.0);
  scene_builder.new_keyframe(60);
// progression 4, reflective floor
  // move sphere and lights and make floor reflective
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).material_idx = MAT_MIRROR;
  scene_builder.get_geometry().get_object(object_ids_b.at(1)).get_spatial_conf().set_position(cm::Vec3(-60.0, 0.0, 0.0));
  // move lights under the sphere and then in front of it
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-50.0, -6.0, -10.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(-50.0, -6.0, 10.0));
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(1000.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(1000.0);
  scene_builder.new_keyframe(60);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-60.0, -6.0, -10.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(-60.0, -6.0, 10.0));
  scene_builder.new_keyframe(60);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-50.0, -6.0, -10.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(-50.0, -6.0, 10.0));
  scene_builder.new_keyframe(60);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-50.0, 4.0, -10.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(-50.0, 4.0, 10.0));
// progression 5, texture cards
  {
    // create the quads for progression 5
    for (uint32_t i = 0; i < 6; i++) object_ids_a.emplace_back(scene_builder.get_geometry().add_object(plane_object));
    for (uint32_t i = 0; i < 6; i++) scene_builder.get_geometry().get_object(object_ids_a.at(i)).get_spatial_conf().rotate(0.0, 90.0, 0.0);
    scene_builder.get_geometry().get_object(object_ids_a.at(0)).get_spatial_conf().set_position(cm::Vec3(-4.0, 2.0, -10.0));
    scene_builder.get_geometry().get_object(object_ids_a.at(1)).get_spatial_conf().set_position(cm::Vec3(0.0, 2.0, -10.0));
    scene_builder.get_geometry().get_object(object_ids_a.at(2)).get_spatial_conf().set_position(cm::Vec3(4.0, 2.0, -10.0));
    scene_builder.get_geometry().get_object(object_ids_a.at(3)).get_spatial_conf().set_position(cm::Vec3(-4.0, -2.0, -10.0));
    scene_builder.get_geometry().get_object(object_ids_a.at(4)).get_spatial_conf().set_position(cm::Vec3(0.0, -2.0, -10.0));
    scene_builder.get_geometry().get_object(object_ids_a.at(5)).get_spatial_conf().set_position(cm::Vec3(4.0, -2.0, -10.0));
  }
  scene_builder.new_keyframe(60);
  // rotate camera to show new scene
  scene_builder.get_camera().get_spatial_conf().set_orientation(cm::quat_look_at(cm::Vec3(0.0, 0.0, -1.0)));
  // move lights over the first two quads and make one global light at the camera position
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-4.0, 2.0, -8.0));
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(50.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(-4.0, -2.0, -8.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(50.0);
  scene_builder.get_lights().get_element(light_ids.at(1)).set_position(cm::Vec3(0.0, 0.0, 0.0));
  scene_builder.get_lights().get_element(light_ids.at(1)).set_intensity(500.0);
  scene_builder.new_keyframe(20);
  remove_objects(scene_builder, object_ids_b);
  // reveal first two textures with a brightness flash of the lights
  scene_builder.new_keyframe(4);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(2000.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(2000.0);
  scene_builder.get_geometry().get_object(object_ids_a.at(0)).material_idx = MAT_CHECKER_PINK_CYAN;
  scene_builder.get_geometry().get_object(object_ids_a.at(3)).material_idx = MAT_CHECKER_PINK_CHECKER_BLACK_WHITE;
  scene_builder.new_keyframe(4);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(50.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(50.0);
  scene_builder.new_keyframe(20);
  scene_builder.new_keyframe(20);
  // move lights to second quads
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(0.0, 2.0, -8.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(0.0, -2.0, -8.0));
  scene_builder.new_keyframe(20);
  // reveal second two textures
  scene_builder.new_keyframe(4);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(2000.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(2000.0);
  scene_builder.get_geometry().get_object(object_ids_a.at(1)).material_idx = MAT_EDGES_PINK_CYAN;
  scene_builder.get_geometry().get_object(object_ids_a.at(4)).material_idx = MAT_EDGES_PINK_CHECKER_BLACK_WHITE;
  scene_builder.new_keyframe(4);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(50.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(50.0);
  scene_builder.new_keyframe(20);
  scene_builder.new_keyframe(20);
  // move lights to third quads
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(4.0, 2.0, -8.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(4.0, -2.0, -8.0));
  scene_builder.new_keyframe(20);
  // reveal third two textures
  scene_builder.new_keyframe(4);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(2000.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(2000.0);
  scene_builder.get_geometry().get_object(object_ids_a.at(2)).material_idx = MAT_DRAGON;
  scene_builder.get_geometry().get_object(object_ids_a.at(5)).material_idx = MAT_CHECKER_FIRE_DRAGON;
  scene_builder.new_keyframe(4);
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(50.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(50.0);
  scene_builder.new_keyframe(20);
  scene_builder.new_keyframe(60);
  // move lights back to illuminate all quads
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-4.0, 0.0, 0.0));
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(500.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(4.0, 0.0, 0.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(500.0);
  scene_builder.new_keyframe(20);
// progression 6, glass sphere cornell
  // create planes of the cornell box for progression 6 and 7
  for (uint32_t i = 0; i < 5; i++) object_ids_b.emplace_back(scene_builder.get_geometry().add_object(plane_object));
  object_ids_b.emplace_back(scene_builder.get_geometry().add_object(sphere_object));
  // right wall
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().set_position(cm::Vec3(50.0, 0.0, -16.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().set_scale(4.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().rotate(0.0, 0.0, 90.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).material_idx = MAT_DIFFUSE_GREEN;
  // ceiling
  scene_builder.get_geometry().get_object(object_ids_b.at(1)).get_spatial_conf().set_position(cm::Vec3(0.0, 50.0, -16.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(1)).get_spatial_conf().set_scale(4.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(1)).get_spatial_conf().rotate(0.0, 0.0, 180.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(1)).material_idx = MAT_DIFFUSE_WHITE;
  // left wall
  scene_builder.get_geometry().get_object(object_ids_b.at(2)).get_spatial_conf().set_position(cm::Vec3(-50.0, 0.0, -16.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(2)).get_spatial_conf().set_scale(4.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(2)).get_spatial_conf().rotate(0.0, 0.0, -90.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(2)).material_idx = MAT_DIFFUSE_RED;
  // floor
  scene_builder.get_geometry().get_object(object_ids_b.at(3)).get_spatial_conf().set_position(cm::Vec3(0.0, -50.0, -16.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(3)).get_spatial_conf().set_scale(4.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(3)).material_idx = MAT_DIFFUSE_WHITE;
  // back wall
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, -10000.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).get_spatial_conf().set_scale(4.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).get_spatial_conf().rotate(0.0, 90.0, 0.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).material_idx = MAT_DIFFUSE_WHITE;
  // glass sphere
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, -9998.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().rotate(90.0, 0.0, 0.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().set_scale(0.5);
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).material_idx = MAT_GLASS;
  scene_builder.new_keyframe(30);
  // move quads from progression 5 outside of the view frustum
  scene_builder.get_lights().get_element(light_ids.at(1)).set_intensity(0.0);
  scene_builder.get_geometry().get_object(object_ids_a.at(0)).get_spatial_conf().translate(cm::Vec3(-10.0, 5.0, 0.0));
  scene_builder.get_geometry().get_object(object_ids_a.at(3)).get_spatial_conf().translate(cm::Vec3(-10.0, -5.0, 0.0));
  scene_builder.get_geometry().get_object(object_ids_a.at(1)).get_spatial_conf().translate(cm::Vec3(0.0, 8.0, 0.0));
  scene_builder.get_geometry().get_object(object_ids_a.at(4)).get_spatial_conf().translate(cm::Vec3(0.0, -8.0, 0.0));
  scene_builder.get_geometry().get_object(object_ids_a.at(2)).get_spatial_conf().translate(cm::Vec3(10.0, 5.0, 0.0));
  scene_builder.get_geometry().get_object(object_ids_a.at(5)).get_spatial_conf().translate(cm::Vec3(10.0, -5.0, 0.0));
  scene_builder.new_keyframe(20);
  remove_objects(scene_builder, object_ids_a);
  // move in right wall
  scene_builder.get_geometry().get_object(object_ids_b.at(0)).get_spatial_conf().set_position(cm::Vec3(4.0, 0.0, -16.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2000.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2000.0));
  scene_builder.new_keyframe(20);
  // move in ceiling
  scene_builder.get_geometry().get_object(object_ids_b.at(1)).get_spatial_conf().set_position(cm::Vec3(0.0, 4.0, -16.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(1)).get_spatial_conf().rotate(180.0, 0.0, 0.0);
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2000.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2000.0));
  scene_builder.new_keyframe(20);
  // move in left wall
  scene_builder.get_geometry().get_object(object_ids_b.at(2)).get_spatial_conf().set_position(cm::Vec3(-4.0, 0.0, -16.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2000.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2000.0));
  scene_builder.new_keyframe(20);
  // move in floor
  scene_builder.get_geometry().get_object(object_ids_b.at(3)).get_spatial_conf().set_position(cm::Vec3(0.0, -4.0, -16.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2000.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().translate(cm::Vec3(0.0, 0.0, 2000.0));
  scene_builder.new_keyframe(40);
  // move in back wall with glass sphere
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, -20.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, -18.0));
  scene_builder.get_lights().get_element(light_ids.at(0)).set_position(cm::Vec3(-3.0, 1.0, -16.0));
  scene_builder.get_lights().get_element(light_ids.at(0)).set_intensity(100.0);
  scene_builder.get_lights().get_element(light_ids.at(2)).set_position(cm::Vec3(3.0, 1.0, -16.0));
  scene_builder.get_lights().get_element(light_ids.at(2)).set_intensity(100.0);
  scene_builder.new_keyframe(180);
  // move glass sphere through the camera
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().set_position(cm::Vec3(0.0, 0.0, 3.0));
  scene_builder.new_keyframe(60);
// progression 7 complex cornell box
  // turn off lights to transition to progression 7
  for (uint32_t id : light_ids) scene_builder.get_lights().get_element(light_ids.at(id)).set_intensity(0.0);
  scene_builder.get_geometry().remove_object(object_ids_b.at(5));
  object_ids_b.at(5) = scene_builder.get_geometry().add_object(dragon_object);
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).material_idx = MAT_GLASS;
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().set_position(cm::Vec3(0.0, -2.0, -18.0));
  scene_builder.get_geometry().get_object(object_ids_b.at(5)).get_spatial_conf().set_scale(0.4);
  scene_builder.get_geometry().get_object(object_ids_b.at(1)).material_idx = MAT_DRAGON;
  scene_builder.get_geometry().get_object(object_ids_b.at(3)).material_idx = MAT_MIRROR;
  scene_builder.get_geometry().get_object(object_ids_b.at(4)).material_idx = MAT_DIFFUSE_WHITE;
  scene_builder.new_keyframe(60);
  // turn lights back on and move them up and down like the wings of the dragon
  scene_builder.get_lights().get_element(0).set_intensity(100.0);
  scene_builder.get_lights().get_element(0).set_position(cm::Vec3(-3.0, -1.0, -16.0));
  scene_builder.get_lights().get_element(1).set_intensity(800.0);
  scene_builder.get_lights().get_element(2).set_intensity(100.0);
  scene_builder.get_lights().get_element(2).set_position(cm::Vec3(3.0, -1.0, -16.0));
  scene_builder.new_keyframe(60);
  scene_builder.get_lights().get_element(0).set_position(cm::Vec3(-3.0, 1.0, -16.0));
  scene_builder.get_lights().get_element(2).set_position(cm::Vec3(3.0, 1.0, -16.0));
  scene_builder.new_keyframe(60);
  scene_builder.get_lights().get_element(0).set_position(cm::Vec3(-3.0, -1.0, -16.0));
  scene_builder.get_lights().get_element(2).set_position(cm::Vec3(3.0, -1.0, -16.0));
  return scene_builder.build_scene();
}

