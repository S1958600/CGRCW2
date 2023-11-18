#include "ImageWriter.h"
#include <fstream>

bool ImageWriter::writePPM(const std::string& filename, const Vec3** pixels, int width, int height) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        return false; // Failed to open file
    }

    // Write PPM header
    outputFile << "P3\n" << width << " " << height << "\n255\n";

    // Write pixel data to file
    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            int ir = static_cast<int>(255.999 * pixels[i][j].r());
            int ig = static_cast<int>(255.999 * pixels[i][j].g());
            int ib = static_cast<int>(255.999 * pixels[i][j].b());

            outputFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    outputFile.close();
    return true;
}
