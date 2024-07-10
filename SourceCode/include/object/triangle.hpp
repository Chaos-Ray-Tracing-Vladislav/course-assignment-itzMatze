#pragma once
#include <vector>
#include "object/vertex.hpp"
#include "renderer/hit_info.hpp"
#include "renderer/ray.hpp"
#include "util/vec.hpp"

class Triangle
{
public:
  Triangle() = default;
  Triangle(uint32_t idx0, uint32_t idx1, uint32_t idx2, const std::vector<Vertex>& vertices);
  void add_normal_to_vertices(std::vector<Vertex>& vertices) const;

  bool intersect(const Ray& ray, HitInfo& hit_info, const std::vector<Vertex>& vertices) const;

  uint32_t vertex_indices[3];
  cm::Vec3 geometric_normal;
};

