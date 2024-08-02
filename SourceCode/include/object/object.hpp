#pragma once

#include <vector>
#include "object/aabb.hpp"
#include "object/triangle.hpp"
#include "object/vertex.hpp"
#include "renderer/hit_info.hpp"
#include "util/spatial_configuration.hpp"

class Object
{
public:
  Object(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const SpatialConfiguration& spatial_conf, int32_t material_idx = -1, bool compute_normals = false);
  Object(const std::vector<Vertex>& vertices, const std::vector<Triangle>& triangles, const SpatialConfiguration& spatial_conf, int32_t material_idx = -1);
  const std::vector<Triangle>& get_triangles() const;
  const std::vector<Vertex>& get_vertices() const;
  AABB get_world_space_aabb() const;
  const SpatialConfiguration& get_spatial_conf() const;
  SpatialConfiguration& get_spatial_conf();
  void set_spatial_conf(const SpatialConfiguration& spatial_configuration);
  bool intersect(const Ray& ray, HitInfo& hit_info) const;

private:
  SpatialConfiguration spatial_conf;
  std::vector<Vertex> vertices;
  std::vector<Triangle> triangles;
  int32_t material_idx;
};

Object interpolate(const Object& a, const Object& b, float weight);
