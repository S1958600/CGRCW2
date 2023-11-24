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



RayTracer::RayTracer() : renderMode(BINARY) {samplesPP = 4; maxDepth = 8;}

RayTracer::RayTracer(RenderMode renderMode, Camera& camera, Scene& scene, int nbounces ) 
: renderMode(renderMode), camera(camera), scene(scene), nbounces(nbounces) {samplesPP = 4; maxDepth = 8;}

RayTracer::RayTracer(RenderMode renderMode, Camera& camera, Scene& scene)
: renderMode(renderMode), camera(camera), scene(scene) {samplesPP = 4; maxDepth = 8;}

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
    camera.setSamples(samplesPP);
    // Generate a list of Ray objects
    Ray* rays = camera.generateAllRays();

    // Create a list of Color objects to store the color of each pixel
    Vec3* image = new Vec3[camera.getHeight() * camera.getWidth()];

    // Calculate the color of each pixel
    for (int y = 0; y < camera.getHeight(); ++y) {
        for (int x = 0; x < camera.getWidth(); ++x) {
            Vec3 colorSum(0, 0, 0);
            for (int s = 0; s < samplesPP; ++s) {
                int index = ((y * camera.getWidth() + x) * samplesPP) + s;
                colorSum += color(rays[index], 0, nullptr);
            }
            int index = y * camera.getWidth() + x;
            image[index] = colorSum / samplesPP;
        }
    }

    delete[] rays;
    return image;
}


Vec3 RayTracer::color(const Ray& r, int depth, Shape* ignoreShape) {
    // If the ray has bounced too many times, no more light is gathered
    if (depth > maxDepth) return scene.getBackgroundColor();

    Hit closestHit;
    Shape* closestShape = nullptr;

    // Find the closest shape that intersects with the ray
    for (const auto& shape : scene.getShapes()) {
        Hit hit;
        if (shape->intersect(r, hit, ignoreShape)) {
            if (closestShape == nullptr || hit.t() < closestHit.t()) {
                closestHit = hit;
                closestShape = shape;
            }
        }
    }

    // If no shape intersects with the ray, return the background color
    if (closestShape == nullptr) {return scene.getBackgroundColor();}

    // If the ray intersects with a shape, calculate the color of the pixel
    //binary simply returns red if there is a hit
    if (renderMode == BINARY){return Vec3(1.0, 0.0, 0.0);}

    else if (renderMode == PHONG){
        double ka = 0.3;
        Vec3 ambient = Vec3(0.3,0.3,0.3) + (closestHit.material()->diffusecolor * 0.3);
        Vec3 diffuse(0.0, 0.0, 0.0);
        Vec3 specular(0.0, 0.0, 0.0);

        // Compute diffuse and specular components for each light source in the scene
        for (const auto& light : scene.getLights()) {
            // Calculate light direction and distance
            Vec3 lightDir = (light->getPosition() - closestHit.location()).make_normalised();
            float lightDistance = (light->getPosition() - closestHit.location()).length();

            // Check if the point is in shadow (i.e., blocked by another object)
            bool inShadow = false;
            Hit shadowHit = Hit();
            // Create a shadow ray from the light towards the hit location
            Ray shadowRay(light->getPosition(), -lightDir);

            for (const auto& shadowShape : scene.getShapes()) {
                //if (shadowShape == closestShape) continue; // Skip the shape that the shadow ray originated from

                if (shadowShape->intersect(shadowRay, shadowHit, closestShape)) {
                    if ((shadowHit.location() - light->getPosition()).length() < lightDistance) {
                        inShadow = true;
                        break;
                    }
                }
            }

            // If not in shadow, compute and add illumination
            if (!inShadow) {
                double diffuseFactor = dot(closestHit.normal(), lightDir);
                Vec3 reflectDir = (lightDir - 2 * dot(lightDir, closestHit.normal()) * closestHit.normal()).make_normalised();
                double specularFactor = pow(std::max(dot(reflectDir, -r.getDirection()), 0.0), closestHit.material()->specularexponent);
                diffuse += closestHit.material()->diffusecolor * light->getIntensity() * std::max(diffuseFactor, 0.0);
                specular += closestHit.material()->specularcolor * light->getIntensity() * specularFactor;
            }
        }

        // Calculate the reflection
        Vec3 reflectionColor = Vec3(0, 0, 0);
        if (closestHit.material()->isreflective) {
            Vec3 normal = closestHit.normal();
            if (dot(normal, r.getDirection()) > 0) {
                normal = -normal;  // Flip the normal
            }
            Vec3 reflectionDir = (r.getDirection() - 2 * dot(r.getDirection(), normal) * normal).make_normalised();
            Vec3 offset = normal * 1e-4;  // Small offset to add to location
            Ray reflectionRay(closestHit.location() + offset, reflectionDir.make_normalised());
            reflectionColor = color(reflectionRay, depth + 1, closestShape);
        }

        if (closestHit.material()->isreflective) {
            //return a combination of the reflection and the phong shading
            //scaled as a ratio of the reflectivity of the material
            Vec3 colorOut = (diffuse * closestHit.material()->kd +
                            specular * closestHit.material()->ks +
                            ambient * ka) * (1 - closestHit.material()->reflectivity) +
                            reflectionColor * closestHit.material()->reflectivity;

            return colorOut;
        }

        // Combine ambient, diffuse, and specular components
        Vec3 colorOut = diffuse * closestHit.material()->kd +
                        specular * closestHit.material()->ks +
                        ambient * ka;

        return colorOut;
    }
    else if (renderMode == PATHTRACER){
        /* code */
    }

    //no rendering option selected or no hit
    return scene.getBackgroundColor();
}




int main() {
    /*
    RayTracer renderer;
    Scene basicScene = Scene();
    Material material = Material( 0.1, 0.9, 10, Vec3(0.5, 1, 0.5),Vec3(1.0,1.0,1.0), false, 1.0, false, 1.0);
    Camera cam = Camera(1200, 800, Vec3(0.0, 0, 0), Vec3(0.0, 0, 1.0), Vec3(0.0, 1.0, 0.0), 45.0, 0.1);
    renderer.setCamera(cam);
    basicScene.setBackgroundColor(Vec3(0.0, 0.0, 0.0));
    //basicScene.addShape(new Sphere(Vec3(0.0, 0.5, 1.0), 0.2, material));
    basicScene.addShape(new Cylinder(Vec3(-0.4, -0.1, 1.3), Vec3(-1.0, 0.0, 0.0), 0.15, 0.2, material));
    basicScene.addShape(new Cylinder(Vec3(0.0, -0.3, 1.3), Vec3(0.0, 0.0, -1.0), 0.15, 0.2, material));
    basicScene.addShape(new Cylinder(Vec3(0.4, -0.1, 1.3), Vec3(0.0, 0.0, 1.0), 0.15, 0.2, material));
    //basicScene.addShape(new Triangle(Vec3(0.0, 0.0, 1.5), Vec3(0.5, 0.0, 1.0), Vec3(0.25, 0.25, 1.0), material));
    basicScene.addLight(new Light(Vec3(0.0, 2.0, 0.0), Vec3(1.0, 1.0, 1.0)));
    renderer.setScene(basicScene);
    renderer.setRenderMode(PHONG);
    Vec3* image = renderer.RenderScene();
    ImageWriter::writePPM("output.ppm", image, 1200, 800);
    delete[] image;
    printf("Image write finished\n");
    */

    
    RayTracer rayTracer = parseRender("imports/scene.json");
    rayTracer.setRenderMode(PHONG);
    Vec3* image2 = rayTracer.RenderScene();
    ImageWriter::writePPM("scene.ppm", image2, 1200, 800);
    delete[] image2;
    
    /*
    Vec3* image3 = parseRender("imports/binary_primitves.json").RenderScene();
    ImageWriter::writePPM("binary_primitives.ppm", image3, 1200, 800);
    delete[] image3;
    */

    Vec3* image4 = parseRender("imports/simple_phong.json").RenderScene();
    ImageWriter::writePPM("simple_phong.ppm", image4, 1200, 800);
    delete[] image4;

    Vec3* image5 = parseRender("imports/mirror_image.json").RenderScene();
    ImageWriter::writePPM("mirror_image.ppm", image5, 1200, 800); 
    delete[] image5;

    return 0;
}