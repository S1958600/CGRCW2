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

double clamp(double min, double max, double value) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}

void fresnel(const Vec3 &I, const Vec3 &N, const float &ior, float &kr)
{
    float cosi = clamp(-1, 1, dot(I, N));
    float etai = 1, etat = ior;
    if (cosi > 0) { std::swap(etai, etat); }
    // Compute sini using Snell's law
    float sint = etai / etat * sqrtf(std::max(0.f, 1 - cosi * cosi));
    // Total internal reflection
    if (sint >= 1) {
        kr = 1;
    }
    else {
        float cost = sqrtf(std::max(0.f, 1 - sint * sint));
        cosi = fabsf(cosi);
        float Rs = ((etat * cosi) - (etai * cost)) / ((etat * cosi) + (etai * cost));
        float Rp = ((etai * cosi) - (etat * cost)) / ((etai * cosi) + (etat * cost));
        kr = (Rs * Rs + Rp * Rp) / 2;
    }
    // As a consequence of the conservation of energy, the transmittance is given by:
    // kt = 1 - kr;
}


Vec3 RayTracer::color(const Ray& r, int depth, Shape* ignoreShape) {
    // If the ray has bounced too many times, no more light is gathered
    if (depth > maxDepth) return scene.getBackgroundColor();

    Hit closestHit;
    Shape* closestShape = nullptr;

    // Find the closest shape that intersects with the ray
    for (const auto& shape : scene.getShapes()) {
        if (shape == ignoreShape) continue;  // Skip the current shape

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
            Vec3 normal = closestHit.normal().make_normalised();
            Vec3 normDir = r.getDirection().make_normalised();

            Vec3 reflectionDir = (normDir - 2.0f * dot(normDir, normal) * normal).make_normalised();
            Vec3 offset = normal * 1e-2;  // Small offset to add to location
            Ray reflectionRay(closestHit.location() + offset, reflectionDir);
            reflectionColor = color(reflectionRay, depth + 1, closestShape);
        }

        // Calculate the refraction
        Vec3 refractionColor = Vec3(0, 0, 0);
        if (closestHit.material()->isrefractive) {
            Vec3 normal = closestHit.normal().make_normalised();
            Vec3 normDir = r.getDirection().make_normalised();
            double eta = closestHit.material()->refractiveindex;

            // Check if the ray is inside the object to invert the normal and invert the refractive index
            if (dot(normDir, normal) > 0) {
                normal = -normal;
                eta = 1 / eta;
            }

            double cosThetaI = -dot(normDir, normal);
            double sin2ThetaT = eta * eta * (1.0 - cosThetaI * cosThetaI);

            // Total internal reflection, no refraction
            if (sin2ThetaT > 1.0) {
                refractionColor = Vec3(0, 0, 0);
            } else {
                Vec3 refractionDir = eta * normDir + (eta * cosThetaI - sqrt(1.0 - sin2ThetaT)) * normal;
                Vec3 offset = -normal * 1e-2;  // Small offset to subtract from location
                Ray refractionRay(closestHit.location() + offset, refractionDir);
                refractionColor = color(refractionRay, depth + 1, closestShape);
            }
        }

        // Calculate Phong shading
        Vec3 colorOut = diffuse * closestHit.material()->kd +
                        specular * closestHit.material()->ks +
                        ambient * ka;

        // Add reflection and refraction
        if (closestHit.material()->isreflective || closestHit.material()->isrefractive) {
            if (closestHit.material()->isreflective && closestHit.material()->isrefractive) {
                // Fresnel effect: the amount of reflection and refraction depends on the angle of incidence
                float kr;
                fresnel(r.getDirection(), closestHit.normal(), closestHit.material()->refractiveindex, kr);

                // Combine reflection and refraction based on Fresnel
                colorOut = colorOut * (1 - kr) + (reflectionColor * kr + refractionColor * (1 - kr));
            } else if (closestHit.material()->isreflective) {
                // Only reflective
                colorOut = colorOut * (1 - closestHit.material()->reflectivity) + reflectionColor * closestHit.material()->reflectivity;
            } else if (closestHit.material()->isrefractive) {
                // Only refractive
                colorOut = colorOut + refractionColor;
            }
        }

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

    

    /*
    RayTracer rayTracer = parseRender("imports/scene.json");
    rayTracer.setRenderMode(PHONG);
    Vec3* image2 = rayTracer.RenderScene();
    ImageWriter::writePPM("scene.ppm", image2, 1200, 800);
    delete[] image2;
    */


    /*
    Vec3* image3 = parseRender("imports/binary_primitves.json").RenderScene();
    ImageWriter::writePPM("binary_primitives.ppm", image3, 1200, 800);
    delete[] image3;
    */

    /*
    Vec3* image4 = parseRender("imports/simple_phong.json").RenderScene();
    ImageWriter::writePPM("simple_phong.ppm", image4, 1200, 800);
    delete[] image4;
    */


    /*
    Vec3* image5 = parseRender("imports/mirror_image.json").RenderScene();
    ImageWriter::writePPM("mirror_image.ppm", image5, 1200, 800); 
    delete[] image5;
    */

    RayTracer renderer2;
    renderer2.setRenderMode(PHONG);
    Scene basicScene2 = Scene();
    basicScene2.setBackgroundColor(Vec3(0.25, 0.25, 0.25));

    // Camera
    Camera cam2 = Camera(1200, 800, Vec3(0.0, 0.75, -0.25), Vec3(0.0, 0.35, 1.0), Vec3(0.0, 1.0, 0.0), 45.0, 0.1);
    renderer2.setCamera(cam2);

    // Lights
    basicScene2.addLight(new Light(Vec3(0, 1.0, 0.5), Vec3(0.5, 0.5, 0.5)));
    basicScene2.addLight(new Light(Vec3(0, 1.0, -0.5), Vec3(0.5, 0.5, 0.5)));

    // Materials
    Material material1 = Material(0.1, 0.9, 10, Vec3(0.5, 1, 0.5), Vec3(1.0,1.0,1.0), false, 1.0, false, 1.0);
    Material material2 = Material(0.1, 0.9, 20, Vec3(0.5, 0.5, 0.8), Vec3(1.0,1.0,1.0), false, 0.5, false, 0.5);
    Material material3 = Material(0.3, 0.9, 2, Vec3(0.8, 0.5, 0.8), Vec3(1.0,1.0,1.0), true, 1.0, false, 1.0);
    Material material4 = Material(0.1, 0.9, 20, Vec3(0.0, 0.0, 0.0), Vec3(1.0,1.0,1.0), false, 0.2, true, 0.95);
    Material material5 = Material(0.1, 0.9, 20, Vec3(0.8, 0.5, 0.5), Vec3(1.0,1.0,1.0), false, 0.3, true, 0.2);

    // Shapes
    basicScene2.addShape(new Sphere(Vec3(0, -25.0, 0), 25.1, material1));
    basicScene2.addShape(new Cylinder(Vec3(-0.3, 0.19, 1), Vec3(0, 1, 0), 0.15, 0.2, material2));
    basicScene2.addShape(new Triangle(Vec3(0, 0.0, 2.25), Vec3(0.75, 0.0, 2), Vec3(0, 0.75, 2.25), material3));
    basicScene2.addShape(new Triangle(Vec3(0.75, 0.75, 2), Vec3(0.75, 0.0, 2), Vec3(0, 0.75, 2.25), material3));
    basicScene2.addShape(new Sphere(Vec3(-0.3, 0.59, 1), 0.2, material4));
    basicScene2.addShape(new Sphere(Vec3(0.3, 0.29, 1), 0.2, material5));

    // Set the scene
    renderer2.setScene(basicScene2);

    // Render the scene
    Vec3* image6 = renderer2.RenderScene();
    ImageWriter::writePPM("custom_scene.ppm", image6, 1200, 800); 
    delete[] image6;


    return 0;
}