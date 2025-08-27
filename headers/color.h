#ifndef COLOR_H
#define COLOR_H

#include "interval.h"

#ifdef USE_SIMD
#include "simd.h"
#else
#include "vec3.h"
#endif

using color = vec3;

/* // unused, was used for writing to ppm files
void write_color(std::ostream& out, const color& pixel_color) { // u
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Translate the [0,1] component values to the byte range [0,255].
    static const interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // Write out the pixel color components.
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
*/

#endif