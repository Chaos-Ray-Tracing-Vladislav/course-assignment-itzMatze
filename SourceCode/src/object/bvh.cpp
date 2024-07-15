#include "object/bvh.hpp"
#include "object/object.hpp"

template<> BVH<Object>::Node::Node(const std::vector<uint32_t>& indices, const std::vector<Object>& objects) : is_leaf(true), indices(indices)
{
  for (uint32_t i : indices)
  {
    bounding_box.min = cm::min(bounding_box.min, objects[i].get_world_space_bounding_box().min);
    bounding_box.max = cm::max(bounding_box.max, objects[i].get_world_space_bounding_box().max);
  }
}
