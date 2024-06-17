#include "scene.hpp"
#include "scene_builder.hpp"

Scene create_single_triangle_scene()
{
  SceneBuilder scene_builder;
  scene_builder.get_geometry().add_triangle(Triangle(
    cm::Vec3(-1.0, -1.0, -5.0),
    cm::Vec3(1.0, -1.0, -5.0),
    cm::Vec3(0.0, 1.0, -5.0)
  ));
  return scene_builder.build_scene();
}

Scene create_triple_triangle_scene()
{
  SceneBuilder scene_builder;
  scene_builder.get_geometry().add_triangle(Triangle(
    cm::Vec3(-1.0, -1.0, -5.0),
    cm::Vec3(1.0, -1.0, -5.0),
    cm::Vec3(0.0, 1.0, -5.0)
  ));
  scene_builder.get_geometry().add_triangle(Triangle(
    cm::Vec3(-2.0, -1.0, -4.0),
    cm::Vec3(0.0, -1.0, -4.0),
    cm::Vec3(-1.0, 1.0, -4.0)
  ));
  scene_builder.get_geometry().add_triangle(Triangle(
    cm::Vec3(0.0, -1.0, -4.0),
    cm::Vec3(2.0, -1.0, -4.0),
    cm::Vec3(1.0, 1.0, -4.0)
  ));
  return scene_builder.build_scene();
}

std::vector<Triangle> add_star(const cm::Vec3& center, float spike_thickness, float tip_length, uint32_t tip_count) {
  std::vector<Triangle> triangles;
  for (uint32_t i = 0; i < tip_count; i++)
  {
    cm::Vec3 vertices[3];
    const float angle0 = (i * 2 * M_PI) / tip_count;
    const float angle1 = angle0 - 0.5 * M_PI;
    const float angle2 = angle0 + 0.5 * M_PI;

    const cm::Vec3 v0 = cm::Vec3(sin(angle0) * tip_length, cos(angle0) * tip_length, 0.0) + center;
    const cm::Vec3 v1 = cm::Vec3(sin(angle1) * spike_thickness, cos(angle1) * spike_thickness, 0.0) + center;
    const cm::Vec3 v2 = cm::Vec3(sin(angle2) * spike_thickness, cos(angle2) * spike_thickness, 0.0) + center;

    triangles.emplace_back(Triangle(v0, v1, v2));
  }
  return triangles;
}

Scene create_pyramid_star_scene()
{
  SceneBuilder scene_builder;
  // left pyramid
  {
    const std::vector<cm::Vec3> vertices{
      cm::Vec3(-2.0, -1.0, -4.0),
      cm::Vec3(0.0, -1.0, -6.0),
      cm::Vec3(-2.0, -1.0, -8.0),
      cm::Vec3(-4.0, -1.0, -6.0),
      cm::Vec3(-2.0, 1.0, -6.0)
    };
    scene_builder.get_geometry().add_triangle(Triangle(vertices[0], vertices[1], vertices[4]));
    scene_builder.get_geometry().add_triangle(Triangle(vertices[1], vertices[2], vertices[4]));
    scene_builder.get_geometry().add_triangle(Triangle(vertices[2], vertices[3], vertices[4]));
    scene_builder.get_geometry().add_triangle(Triangle(vertices[3], vertices[0], vertices[4]));
  }
  // right pyramid
  {
    const std::vector<cm::Vec3> vertices{
      cm::Vec3(2.0, -1.0, -4.0),
      cm::Vec3(4.0, -1.0, -6.0),
      cm::Vec3(2.0, -1.0, -8.0),
      cm::Vec3(0.0, -1.0, -6.0),
      cm::Vec3(2.0, 1.0, -6.0)
    };
    scene_builder.get_geometry().add_triangle(Triangle(vertices[0], vertices[1], vertices[4]));
    scene_builder.get_geometry().add_triangle(Triangle(vertices[1], vertices[2], vertices[4]));
    scene_builder.get_geometry().add_triangle(Triangle(vertices[2], vertices[3], vertices[4]));
    scene_builder.get_geometry().add_triangle(Triangle(vertices[3], vertices[0], vertices[4]));
  }

  // star
  scene_builder.get_geometry().add_triangles(add_star(cm::Vec3(0.0, 0.5, -4.0), 0.1, 0.5, 5));

  return scene_builder.build_scene();
}

Scene hw06_task02()
{
  SceneBuilder scene_builder;
  scene_builder.get_geometry().add_triangle(Triangle(
    {cm::Vec3(-1.75, -1.75, -3.0)},
    {cm::Vec3(1.75, -1.75, -3.0)},
    {cm::Vec3(0.0, 1.75, -3.0)}
  ));
  scene_builder.get_camera().set_origin(cm::Vec3(-4.0, 0.0, 1.0));
  scene_builder.get_camera().set_view_dir(cm::Vec3(0.0, 0.0, -3.0) - scene_builder.get_camera().get_origin());
  return scene_builder.build_scene();
}

