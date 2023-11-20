#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

//#include "ImportParser.cpp"
#include "RayStructures.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "ImageWriter.h"
#include "Scene.h"
#include "Material.h"


class RayTracer {
private:
    Scene scene;

public:
    RayTracer(/* constructor parameters */) {
        // Initialize rendering parameters and scene.
    }

    void setScene(Scene& scene) {
        this->scene = scene;
    }


    Vec3* RenderScene1D() {
        // Get the Camera object from the Scene object
        Camera camera = scene.getCamera();

        // Generate a 1D list of Ray objects
        Ray* rays = camera.generateAllRays1D(camera.getWidth(), camera.getHeight());

        // Create a 1D list of Color objects to store the color of each pixel
        Vec3* image = new Vec3[camera.getHeight() * camera.getWidth()];

        // Calculate the color of each pixel
        for (int y = 0; y < camera.getHeight(); ++y) {
            for (int x = 0; x < camera.getWidth(); ++x) {
                int index = y * camera.getWidth() + x;
                image[index] = color(rays[index]);
            }
        }

        return image;
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

        Hit hit = Hit();
        for (const auto& shape : scene.getShapes()) {
            if (shape->intersect(r, hit)) {
                return Vec3(10.0, 0.0, 0.0);
            }
        }
        return scene.getBackgroundColor();
    }
};



int main() {
    // Set up scene and rendering options.
    RayTracer renderer;

    //parseJson("imports/binary_primitives.json");

    Scene basicScene = Scene();
    printf("Scene created\n");

    Material material = Material();

    basicScene.setCamera(Camera(1200, 800, Vec3(0.0, 0, 0), Vec3(0.0, 0, 1.0), Vec3(0.0, 1.0, 0.0), 45.0, 0.1));
    basicScene.setBackgroundColor(Vec3(0.25, 0.25, 1.0));
    basicScene.addShape(new Sphere(Vec3(0.0, 0.5, 1.0), 0.2, material));
    basicScene.addShape(new Cylinder(Vec3(-0.5, 0.0, 0.75), Vec3(1.0, 0.0, 0.0), 0.15, 0.2, material));
    basicScene.addShape(new Cylinder(Vec3(0.0, 0.0, 0.75), Vec3(0.0, 1.0, 0.0), 0.15, 0.2, material));
    basicScene.addShape(new Cylinder(Vec3(0.5, 0.0, 0.75), Vec3(0.0, 0.0, 1.0), 0.15, 0.2, material));


    printf("Scene populated\n");

    renderer.setScene(basicScene);

    Vec3* image = renderer.RenderScene1D();
    printf("Scene rendered\n");


    ImageWriter::writePPM("output.ppm", image, 1200, 800);
    
    printf("Image write exited\n");

    //renderer.RenderScene(/* rendering options */);
    //renderer.SavePPMImage("output.ppm");

    return 0;
}


/*
{
    "rendermode":"binary",
    "camera":
        { 
            "type":"pinhole", 
            "width":1200, 
            "height":800,
            "position":[0.0, 0, 0],
            "lookAt":[0.0, 0, 1.0],
            "upVector":[0.0, 1.0, 0.0],
            "fov":45.0,
            "exposure":0.1
        },
    "scene":
        { 
            "backgroundcolor": [0.25, 0.25, 0.25], 
            "shapes":[ 
                { 
                    "type":"sphere", 
                    "center": [-0.3, 0.19, 1],
                    "radius":0.2
                },
                {
                    "type": "cylinder",
                    "center": [-0.3, -0.2, 1],
                    "axis": [1, 0, 0],
                    "radius": 0.15,
                    "height": 0.2
                },
                { 
                    "type":"triangle", 
                    "v0": [0, 0, 1],
                    "v1": [0.5, 0, 1],
                    "v2": [0.25,  0.25, 1]
                }  
            ] 
        } 
}*/