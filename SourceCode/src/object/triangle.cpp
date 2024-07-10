#include "object/triangle.hpp"

#define EPSILON 0.0001

Triangle::Triangle(uint32_t idx0, uint32_t idx1, uint32_t idx2, const std::vector<Vertex>& vertices)
{
  vertex_indices[0] = idx0;
  vertex_indices[1] = idx1;
  vertex_indices[2] = idx2;
  geometric_normal = cm::normalize(cm::cross(cm::normalize(vertices[idx1].pos - vertices[idx0].pos), cm::normalize(vertices[idx2].pos - vertices[idx0].pos)));
}

void Triangle::add_normal_to_vertices(std::vector<Vertex>& vertices) const
{
  vertices[vertex_indices[0]].normal += geometric_normal;
  vertices[vertex_indices[1]].normal += geometric_normal;
  vertices[vertex_indices[2]].normal += geometric_normal;
}

bool Triangle::intersect(const Ray& ray, HitInfo& hit_info, const std::vector<Vertex>& vertices) const
{
  const Vertex& v0 = vertices[vertex_indices[0]];
  const Vertex& v1 = vertices[vertex_indices[1]];
  const Vertex& v2 = vertices[vertex_indices[2]];
  const float dot_n_dir = cm::dot(ray.dir, geometric_normal);
  // backface culling
  if (ray.config.backface_culling && dot_n_dir >= 0.0) return false;
  hit_info.t = (cm::dot(geometric_normal, v0.pos) - cm::dot(ray.origin, geometric_normal)) / dot_n_dir;
  // hit is behind the ray or outside of allowed range
  if (hit_info.t < 0.0 || hit_info.t > ray.config.max_t) return false;
  hit_info.pos = ray.at(hit_info.t);
  // check if point lies inside or outside of the triangle
  {
    const cm::Vec3 e = cm::normalize(v1.pos - v0.pos);
    const cm::Vec3 vp = cm::normalize(hit_info.pos - v0.pos);
    if (cm::dot(geometric_normal, cm::cross(e, vp)) < -EPSILON) return false;
  }
  {
    const cm::Vec3 e = cm::normalize(v2.pos - v1.pos);
    const cm::Vec3 vp = cm::normalize(hit_info.pos - v1.pos);
    if (cm::dot(geometric_normal, cm::cross(e, vp)) < -EPSILON) return false;
  }
  {
    const cm::Vec3 e = cm::normalize(v0.pos - v2.pos);
    const cm::Vec3 vp = cm::normalize(hit_info.pos - v2.pos);
    if (cm::dot(geometric_normal, cm::cross(e, vp)) < -EPSILON) return false;
  }
  hit_info.bary.u = cm::length(cm::cross(hit_info.pos - v0.pos, v2.pos - v0.pos)) / cm::length(cm::cross(v1.pos - v0.pos, v2.pos - v0.pos));
  hit_info.bary.v = cm::length(cm::cross(hit_info.pos - v0.pos, v1.pos - v0.pos)) / cm::length(cm::cross(v1.pos - v0.pos, v2.pos - v0.pos));
  hit_info.geometric_normal = geometric_normal;
  hit_info.normal = cm::normalize(hit_info.bary.u * v1.normal + hit_info.bary.v * v2.normal + (1.0 - hit_info.bary.u - hit_info.bary.v) * v0.normal);
  return true;
}
