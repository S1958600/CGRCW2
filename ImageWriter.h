#ifndef IMAGE_WRITER_H
#define IMAGE_WRITER_H

#include <string>
#include "Vec3.h"

class ImageWriter {
public:
    // Method to write a PPM image from pixel data
    static bool writePPM(const std::string& filename, Vec3** pixels, int width, int height);

    static bool writePPM(const std::string& filename, Vec3* pixels, int width, int height);
};

#endif // IMAGE_WRITER_H
