#include "scene.hpp"

Scene create_single_triangle_scene()
{
  Scene scene;
  scene.add_triangle(Triangle(
    cm::Vec3(-1.0, -1.0, -5.0),
    cm::Vec3(1.0, -1.0, -5.0),
    cm::Vec3(0.0, 1.0, -5.0)
  ));
  return scene;
}

Scene create_triple_triangle_scene()
{
  Scene scene;
  scene.add_triangle(Triangle(
    cm::Vec3(-1.0, -1.0, -5.0),
    cm::Vec3(1.0, -1.0, -5.0),
    cm::Vec3(0.0, 1.0, -5.0)
  ));
  scene.add_triangle(Triangle(
    cm::Vec3(-2.0, -1.0, -4.0),
    cm::Vec3(0.0, -1.0, -4.0),
    cm::Vec3(-1.0, 1.0, -4.0)
  ));
  scene.add_triangle(Triangle(
    cm::Vec3(0.0, -1.0, -4.0),
    cm::Vec3(2.0, -1.0, -4.0),
    cm::Vec3(1.0, 1.0, -4.0)
  ));
  return scene;
}

