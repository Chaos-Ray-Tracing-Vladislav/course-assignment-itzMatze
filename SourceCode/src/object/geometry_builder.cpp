#include "object/geometry_builder.hpp"

GeometryBuilder::GeometryBuilder(const InterpolatableData<Object>& objects, const std::vector<Material>& materials) : objects(objects), materials(materials)
{}

uint32_t GeometryBuilder::add_object(const Object& object)
{
  return objects.add_new_data(object);
}

void GeometryBuilder::add_material(const Material& material)
{
  materials.emplace_back(material);
}

InterpolatableData<Object>& GeometryBuilder::get_interpolatable_objects()
{
  return objects;
}

Object& GeometryBuilder::get_object(uint32_t id)
{
  return objects.get_element(id);
}

void GeometryBuilder::remove_object(uint32_t id)
{
  objects.remove_element(id);
}

Geometry GeometryBuilder::build_geometry()
{
  return Geometry(objects, materials);
}
