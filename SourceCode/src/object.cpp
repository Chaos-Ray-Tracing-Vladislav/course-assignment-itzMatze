#include "object.hpp"
#include "spatial_configuration.hpp"
#include "triangle.hpp"
#include "vec.hpp"

// only triangles are supported
Object::Object(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const SpatialConfiguration& spatial_conf, int32_t material_idx, bool compute_normals) :
  vertices(vertices), spatial_conf(spatial_conf), material_idx(material_idx)
{
  assert(indices.size() % 3 == 0);
  for (uint32_t i = 0; i < indices.size(); i += 3)
  {
    triangles.emplace_back(indices[i], indices[i + 1], indices[i + 2], vertices);
  }
  if (compute_normals)
  {
    for (const auto& triangle : this->triangles) triangle.add_normal_to_vertices(this->vertices);
    for (auto& vertex : this->vertices)
    {
      // make sure that any normals have been added to the vertex
      if (cm::length(vertex.normal) > 0.0001) vertex.normal = cm::normalize(vertex.normal);
    }
  }
}

Object::Object(const std::vector<Vertex>& vertices, const std::vector<Triangle>& triangles, const SpatialConfiguration& spatial_conf, int32_t material_idx) :
  vertices(vertices), triangles(triangles), spatial_conf(spatial_conf), material_idx(material_idx)
{}

const std::vector<Triangle>& Object::get_triangles() const
{
  return triangles;
}

const std::vector<Vertex>& Object::get_vertices() const
{
  return vertices;
}

const SpatialConfiguration& Object::get_spatial_conf() const
{
  return spatial_conf;
}

SpatialConfiguration& Object::get_spatial_conf()
{
  return spatial_conf;
}

bool Object::intersect(const Ray& ray, HitInfo& hit_info) const
{
  HitInfo cur_hit_info;
  // transform ray into local coordinate system of object
  const Ray transformed_ray(spatial_conf.inverse_transform_pos(ray.origin), spatial_conf.inverse_transform_dir(ray.dir));
  for (const auto& triangle : triangles)
  {
    // test if object is intersected and if yes whether the intersection is closer than the previous ones
    if (triangle.intersect(transformed_ray, cur_hit_info, vertices) && (cur_hit_info.t < hit_info.t))
    {
      hit_info = cur_hit_info;
      if (ray.anyhit) return true;
    }
  }
  // if an intersection was found, t is the distance to this intersection instead of maximum float value
  if (hit_info.t < ray.max_t)
  {
    // transform normals
    hit_info.geometric_normal = spatial_conf.transform_dir(hit_info.geometric_normal);
    hit_info.normal = spatial_conf.transform_dir(hit_info.normal);
    hit_info.material_idx = material_idx;
    return true;
  }
  return false;
}

Object interpolate(const Object& a, const Object& b, float weight)
{
  SpatialConfiguration spatial_conf = interpolate(a.get_spatial_conf(), b.get_spatial_conf(), weight);
  return Object(a.get_vertices(), a.get_triangles(), spatial_conf);
}
