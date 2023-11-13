#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>


class RayTracer {
public:
    RayTracer(/* constructor parameters */) {
        // Initialize rendering parameters and scene.
    }

    void RenderScene(/* rendering options */) {
        // Rendering logic: generate pixel colors, shading, etc.
    }

    // Other rendering-related methods...


    //TODO: Read in render output
    void SavePPMImage(const std::string& filename) {
        // Image dimensions (width and height)
        const int width = 800;
        const int height = 600;

        // Create a PPM image file
        std::ofstream imageFile("output.ppm");
        if (!imageFile.is_open()) {
            std::cerr << "Error: Unable to create the PPM file." << std::endl;
            //return error
        }

        // PPM header
        imageFile << "P3\n";          // P3 indicates a color image in ASCII
        imageFile << width << " " << height << "\n";  // Width and height
        imageFile << "255\n";         // Maximum color value (usually 255)

        // Generate a solid color for the image (e.g., red)
        int red = 0;
        int green = 0;
        int blue = 0;

        // Fill the image with the solid color
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                red = int((float(i)/float(height)) * 255) ;
                green = int((float(j)/float(width)) * 255) ;
                blue = 150;
                printf("%d %d %d\n", red, green, blue);
                imageFile << red << " " << green << " " << blue << " ";
            }
            imageFile << "\n";
        }

        // Close the PPM file
        imageFile.close();

        std::cout << "Image created successfully." << std::endl;
        //return success
    }
};

int main() {
    // Set up scene and rendering options.
    RayTracer renderer;

    renderer.RenderScene(/* rendering options */);
    renderer.SavePPMImage("output.ppm");

    return 0;
}