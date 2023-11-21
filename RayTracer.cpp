#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>

#include "ImportParser.h"
#include "RayStructures.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Triangle.h"
#include "ImageWriter.h"
#include "Scene.h"
#include "Material.h"
#include "RayTracer.h"



RayTracer::RayTracer() : renderMode(BINARY) {}

RayTracer::RayTracer(RenderMode renderMode, Camera& camera, Scene& scene, int nbounces ) 
: renderMode(renderMode), camera(camera), scene(scene), nbounces(nbounces) {}

RayTracer::RayTracer(RenderMode renderMode, Camera& camera, Scene& scene)
: renderMode(renderMode), camera(camera), scene(scene) {nbounces = 0;}

void RayTracer::setScene(Scene& scene) {
    this->scene = scene;
}

void RayTracer::setCamera(Camera& camera) {
    this->camera = camera;
}

void RayTracer::setRenderMode(RenderMode renderMode) {
    this->renderMode = renderMode;
}


Vec3* RayTracer::RenderScene() {

    // Generate a list of Ray objects
    Ray* rays = camera.generateAllRays(camera.getWidth(), camera.getHeight());

    // Create a list of Color objects to store the color of each pixel
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


Vec3 RayTracer::color(const Ray& r) {
    Hit hit = Hit();

    if (renderMode == BINARY){
        for (const auto& shape : scene.getShapes()) {
            if (shape->intersect(r, hit)) {
                return Vec3(1.0, 0.0, 0.0);
            }
        }
    }
    else if (renderMode == PHONG){
        for (const auto& shape : scene.getShapes()) {
            if (shape->intersect(r, hit)) {
                
                return Vec3(1.0, 0.0, 0.0);
            }
        }
    }
    else if (renderMode == PATHTRACER){
        /* code */
    }

    //no rendering option selected or no hit
    return scene.getBackgroundColor();
}




int main() {
    // Set up scene and rendering options.
    RayTracer renderer;

    //parseJson("imports/binary_primitives.json");

    Scene basicScene = Scene();

    Material material = Material();

    Camera cam = Camera(1200, 800, Vec3(0.0, 0, 0), Vec3(0.0, 0, 1.0), Vec3(0.0, 1.0, 0.0), 45.0, 0.1);
    renderer.setCamera(cam);

    basicScene.setBackgroundColor(Vec3(0.0, 0.0, 0.0));
    //basicScene.addShape(new Sphere(Vec3(0.0, 0.5, 1.0), 0.2, material));
    basicScene.addShape(new Cylinder(Vec3(-0.5, -0.1, 1), Vec3(1.0, 0.0, 0.0), 0.15, 0.2, material));
    basicScene.addShape(new Cylinder(Vec3(0.0, -0.1, 1), Vec3(0.0, 1.0, 0.0), 0.15, 0.2, material));
    basicScene.addShape(new Cylinder(Vec3(0.5, -0.1, 1), Vec3(0.0, 0.0, 1.0), 0.15, 0.2, material));
    //basicScene.addShape(new Triangle(Vec3(0.0, 0.0, 1.0), Vec3(0.5, 0.0, 1.0), Vec3(0.25, 0.25, 1.0), material));


    printf("Scene populated\n");

    renderer.setScene(basicScene);

    Vec3* image = renderer.RenderScene();
    printf("Scene rendered\n");


    ImageWriter::writePPM("output.ppm", image, 1200, 800);
    
    printf("Image write finished\n");

    
    image = parseRender("imports/scene.json").RenderScene();
    ImageWriter::writePPM("scene.ppm", image, 1200, 800);

    image = parseRender("imports/binary_primitves.json").RenderScene();
    ImageWriter::writePPM("binary_primitives.ppm", image, 1200, 800);

    return 0;
}