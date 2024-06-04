#pragma once
#include <cstdint>
#include <vector>
#include "color.hpp"

std::vector<Color> create_random_color_rectangles_image(const uint32_t width, const uint32_t height, const uint32_t rectangle_count_x = 4, const uint32_t rectangle_count_y = 4);
std::vector<Color> create_fix_color_rectangles_image(const uint32_t width, const uint32_t height, const uint32_t rectangle_count_x = 4, const uint32_t rectangle_count_y = 4);
std::vector<Color> create_circle_image(const int width, const int height, const uint32_t radius = 200);

