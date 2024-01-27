#include "ImageWriter.h"
#include <fstream>


bool ImageWriter::writePPM(const std::string& filename, Vec3* pixels, int width, int height) {
    std::ofstream outputFile("outputs/" + filename);
    if (!outputFile.is_open()) {
        return false; // Failed to open file
    }

    // Write PPM header
    outputFile << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            int index = j * width + i;

            if (&pixels[index] == nullptr ) {
                // Error: Invalid pixel data
                outputFile.close();
                printf("Invalid pixel data. %d x %d\n", i, j);
                return false;
            }

            // Contrast factor
            double a = 2.0;

            // Apply contrast adjustment
            double r = a * (pixels[index].r() - 0.5) + 0.5;
            double g = a * (pixels[index].g() - 0.5) + 0.5;
            double b = a * (pixels[index].b() - 0.5) + 0.5;

            // Clamp values to [0, 1]
            r = std::max(0.0, std::min(1.0, r));
            g = std::max(0.0, std::min(1.0, g));
            b = std::max(0.0, std::min(1.0, b));

            int ir = static_cast<int>(255.999 * pixels[index].r());
            int ig = static_cast<int>(255.999 * pixels[index].g());
            int ib = static_cast<int>(255.999 * pixels[index].b());

            outputFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    printf("Image created successfully.\n");
    outputFile.close();
    return true;
}

