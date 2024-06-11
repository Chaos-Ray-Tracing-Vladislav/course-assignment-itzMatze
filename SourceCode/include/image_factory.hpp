#pragma once
#include <cstdint>
#include <vector>
#include "color.hpp"

std::vector<Color> create_random_color_rectangles_image(const cm::Vec2u resolution, const uint32_t rectangle_count_x = 4, const uint32_t rectangle_count_y = 4);
std::vector<Color> create_fix_color_rectangles_image(const cm::Vec2u resolution, const uint32_t rectangle_count_x = 4, const uint32_t rectangle_count_y = 4);
std::vector<Color> create_circle_image(const cm::Vec2u resolution, const uint32_t radius = 200);
std::vector<Color> create_camera_ray_vis_image(const cm::Vec2u resolution);

