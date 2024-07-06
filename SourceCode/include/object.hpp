#pragma once

#include <vector>
#include "triangle.hpp"
#include "hit_info.hpp"
#include "vertex.hpp"

class Object
{
public:
  Object() = default;
  Object(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, bool compute_normals);
  const std::vector<Triangle>& get_triangles() const;
  const std::vector<Vertex>& get_vertices() const;
  bool intersect(const Ray& ray, HitInfo& hit_info) const;

private:
  std::vector<Triangle> triangles;
  std::vector<Vertex> vertices;
};

Object interpolate(const Object& a, const Object& b, float weight);
