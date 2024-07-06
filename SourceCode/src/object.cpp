#include "object.hpp"
#include "triangle.hpp"
#include "vec.hpp"

// only triangles are supported
Object::Object(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, bool compute_normals) : vertices(vertices)
{
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

const std::vector<Triangle>& Object::get_triangles() const
{
  return triangles;
}

const std::vector<Vertex>& Object::get_vertices() const
{
  return vertices;
}

bool Object::intersect(const Ray& ray, HitInfo& hit_info) const
{
  HitInfo cur_hit_info;
  for (const auto& triangle : triangles)
  {
    // test if object is intersected and if yes whether the intersection is closer than the previous ones
    if (triangle.intersect(ray, cur_hit_info, vertices) && (cur_hit_info.t < hit_info.t))
    {
      hit_info = cur_hit_info;
    }
  }
  // if an intersection was found, t is the distance to this intersection instead of maximum float value
  return (hit_info.t < std::numeric_limits<float>::max());
}

Object interpolate(const Object& a, const Object& b, float weight)
{
  // TODO(Matze): use spatial configuration to interpolate geometry positions
  return a;
}
