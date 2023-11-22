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
        Vec3 color = scene.getBackgroundColor(); // Default background color
        if (scene.getShapes().empty()) {
            return color; // No shapes in the scene
        }

        for (const auto& shape : scene.getShapes()) {
            if (shape->intersect(r, hit)) {
                //Vec3 ambient = hit.material()->ambientColor();   // Ambient color not implemented
                Vec3 diffuse = hit.material()->diffusecolor;
                Vec3 specular = hit.material()->specularcolor;

                // Compute diffuse and specular components for each light source in the scene
                for (const auto& light : scene.getLights()) {
                    // Calculate light direction and distance
                    Vec3 lightDir = (light->getPosition() - hit.location()).make_normalised();
                    float lightDistance = (light->getPosition() - hit.location()).length();

                    // Check if the point is in shadow (i.e., blocked by another object)
                    bool inShadow = false;
                    Hit shadowHit = Hit();
                    Ray shadowRay(hit.location(), lightDir);
                    for (const auto& shadowShape : scene.getShapes()) {
                        if (shadowShape->intersect(shadowRay, shadowHit)) {
                            if ((hit.location() - shadowRay.getOrigin()).length() < lightDistance) {
                                inShadow = true;
                                break;
                                // in shadow, no need to check other objects
                            }
                        }
                    }

                    // If not in shadow, compute illumination
                    if (!inShadow) {
                        float diffuseFactor = dot(hit.normal(), lightDir);
                        Vec3 reflectDir = (lightDir - 2 * dot(lightDir, hit.normal()) * hit.normal()).make_normalised();
                        float specularFactor = pow(std::max(dot(reflectDir, r.getDirection()), 0.0), hit.material()->specularexponent);

                        diffuse += light->getIntensity() * std::max(diffuseFactor, 0.0f);
                        specular += light->getIntensity() * specularFactor;
                    }
                }

                // Combine ambient, diffuse, and specular components
                Vec3 color = diffuse * hit.material()->kd +
                             specular * hit.material()->ks;
                             //ambient * hit.material().getAmbientCoefficient() +

                return color;
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

    image = parseRender("imports/simple_phong.json").RenderScene();
    ImageWriter::writePPM("simple_phong.ppm", image, 1200, 800);

    return 0;
}