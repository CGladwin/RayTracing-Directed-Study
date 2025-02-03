#ifndef COLOR_HPP
#define COLOR_HPP

#include "vec3.hpp"
#include <vector>

#include <iostream>
#include <algorithm>

using color = vec3;

void write_color(std::vector<u_int8_t>& pixels, const color& pixel_color) {
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255]
    // 
    auto rbyte = static_cast<int>(256 * std::clamp(r, 0.0, 0.999));
    auto gbyte = static_cast<int>(256 * std::clamp(g, 0.0, 0.999));
    auto bbyte = static_cast<int>(256 * std::clamp(b, 0.0, 0.999));

    // Write out the pixel color components to vector
    pixels.push_back(rbyte);
    pixels.push_back(gbyte);
    pixels.push_back(bbyte);

}

#endif