#pragma once

#include <iostream>

#include "vec3.h"

using color = vec3;

inline void write_color(std::ostream& out, const color& pixel_color)
{
    const double r = pixel_color.x();
    const double g = pixel_color.y();
    const double b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255]
    const int rbyte = static_cast<int>(255.999 * r);
    const int gbyte = static_cast<int>(255.999 * g);
    const int bbyte = static_cast<int>(255.999 * b);

    // Write out the pixel color components
    out << rbyte << " " << gbyte << " " << bbyte << "\n";
}