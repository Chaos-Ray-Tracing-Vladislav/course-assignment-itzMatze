#pragma once

#include <cassert>
#include <cstdint>
#include <vector>
#include <stdexcept>

template<typename T>
concept Interpolatable = requires(const T& a, const T& b, float weight)
{
  { interpolate(a, b, weight) } -> std::same_as<T>;
};

// stores a list of interpolatable elements to interpolate between two lists of elements properly
// single elements are interpolated trivially and should not use this class
template<Interpolatable T>
class InterpolatableData
{
public:
  InterpolatableData() = default;

  void add_new_data(const T& datum)
  {
    // assign ascending indices to objects
    uint32_t id = ids.size() == 0 ? 0 : ids.back() + 1;
    data.emplace_back(datum);
  }

  void add_data_with_id(const T& datum, uint32_t id)
  {
    // make sure the ids are unique
#if !defined(NDEBUG)
    for (const auto& id_it : ids)
    {
      if (id_it == id) throw std::runtime_error("Error: Duplicate object id!");
    }
    if (ids.size() > 0) assert(ids.back() < id);
#endif
    data.emplace_back(datum);
  }

  const std::vector<T>& get_data() const
  {
    return data;
  }

  const std::vector<uint32_t>& get_ids() const
  {
    return ids;
  }

private:
  std::vector<T> data;
  std::vector<uint32_t> ids;
};

template<typename T>
InterpolatableData<T> interpolate(const InterpolatableData<T>& a, const InterpolatableData<T>& b, float weight)
{
  InterpolatableData<T> result;
  const std::vector<T>& data_a = a.get_data();
  const std::vector<uint32_t>& ids_a = a.get_ids();
  const std::vector<T>& data_b = b.get_data();
  const std::vector<uint32_t>& ids_b = b.get_ids();

  assert(data_a.size() == ids_a.size() && data_b.size() == ids_b.size());
  for (uint32_t i = 0, j = 0; i < data_a.size() && j < data_b.size();)
  {
    if (ids_a[i] == ids_b[j])
    {
      result.add_data_with_id(interpolate(data_a[i], data_b[i], weight), ids_a[i]);
      i++;
      j++;
    }
    else if (ids_a[i] < ids_b[j])
    {
      // add data from previous keyframe that are not present in the next keyframe
      result.add_data_with_id(data_a[i], ids_a[i]);
      i++;
    }
    // additional data in the next keyframe will be added when that keyframe is reached
    else if (ids_a[i] > ids_b[j]) j++;
  }
  return result;
}

