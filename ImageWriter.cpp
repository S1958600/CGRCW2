#include "ImageWriter.h"
#include <fstream>

bool ImageWriter::writePPM(const std::string& filename, Vec3** pixels, int width, int height) {
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        return false; // Failed to open file
    }

    // Write PPM header
    outputFile << "P3\n" << width << " " << height << "\n255\n";

    for (int j = height - 1; j >= 00; --j) {
        for (int i = 0; i < width; ++i) {
            if (pixels[i] == nullptr ) {
                // Error: Invalid pixel data
                outputFile.close();
                printf("Invalid pixel data. %d x %d\n", i, j);
                return false;
            }

            //printf("Writing pixel (%d, %d)\n", i, j);

            int ir = static_cast<int>(255.999 * pixels[i][j].r());
            int ig = static_cast<int>(255.999 * pixels[i][j].g());
            int ib = static_cast<int>(255.999 * pixels[i][j].b());

            outputFile << ir << " " << ig << " " << ib << "\n";
        }
    }

    printf("Image created successfully.\n");
    outputFile.close();
    return true;
}

bool ImageWriter::writePPM(const std::string& filename, Vec3* pixels, int width, int height) {
    std::ofstream outputFile(filename);
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

