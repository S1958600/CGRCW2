#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "ImportParser.cpp"
#include "RayStructures.h"


class RayTracer {
public:
    RayTracer(/* constructor parameters */) {
        // Initialize rendering parameters and scene.
    }

    Vec3** RayTracer::RenderScene(const Scene& scene) {
        // Get the Camera object from the Scene object
        Camera camera = scene.getCamera();

        // Generate a 2D list of Ray objects
        Ray** rays = camera.generateAllRays(camera.getWidth(), camera.getHeight());

        // Create a 2D list of Color objects to store the color of each pixel
        Vec3** image = new Vec3*[camera.getHeight()];
        for (int i = 0; i < camera.getHeight(); ++i) {
            image[i] = new Vec3[camera.getWidth()];
        }

        // Calculate the color of each pixel
        for (int y = 0; y < camera.getHeight(); ++y) {
            for (int x = 0; x < camera.getWidth(); ++x) {
                image[y][x] = color(rays[y][x]);
            }
        }

        return image;

        // TODO: Use the 2D list of Color objects to create the image
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

    Vec3 color(const Ray& r) {
        Vec3 unit_dir = r.getDirection().make_normalised();
        float t = 0.5*(unit_dir.y() + 1.0);
        return (1.0-t)*Vec3(1.0,1.0,1.0)+t*Vec3(0.5,0.7,1.0);
    }
};



int main() {
    // Set up scene and rendering options.
    RayTracer renderer;

    //parseJson("imports/binary_primitives.json");




    //renderer.RenderScene(/* rendering options */);
    //renderer.SavePPMImage("output.ppm");

    return 0;
}