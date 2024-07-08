#pragma once

#include <vector>
#include "spatial_configuration.hpp"
#include "triangle.hpp"
#include "hit_info.hpp"
#include "vertex.hpp"

class Object
{
public:
  Object(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices, const SpatialConfiguration& spatial_conf, bool compute_normals);
  Object(const std::vector<Vertex>& vertices, const std::vector<Triangle>& triangles, const SpatialConfiguration& spatial_conf);
  const std::vector<Triangle>& get_triangles() const;
  const std::vector<Vertex>& get_vertices() const;
  const SpatialConfiguration& get_spatial_conf() const;
  SpatialConfiguration& get_spatial_conf();
  void set_spatial_conf(const SpatialConfiguration& spatial_configuration);
  bool intersect(const Ray& ray, HitInfo& hit_info) const;

private:
  SpatialConfiguration spatial_conf;
  std::vector<Vertex> vertices;
  std::vector<Triangle> triangles;
};

Object interpolate(const Object& a, const Object& b, float weight);
