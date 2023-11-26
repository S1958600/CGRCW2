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
#include "BVH.h"



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

void RayTracer::setSamplesPP(int samplesPP) {
    this->samplesPP = samplesPP;
}

void RayTracer::setMaxDepth(int maxDepth) {
    this->maxDepth = maxDepth;
}

void RayTracer::setBVH(BVH bvh) {
    this->bvh = bvh;
}


Vec3* RayTracer::RenderScene() {
    camera.setSamples(samplesPP);
    Ray* rays = camera.generateAllRays();

    setBVH(BVH(scene.getShapes())); //set up bvh for the scene

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

    /*
    closestHit = bvh.getIntersect(r, bvh.root);
    closestShape = closestHit.shape();
    if (closestHit.t() < 0) {return scene.getBackgroundColor();} //no hit
    */
    
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
            double lightDistance = (light->getPosition() - closestHit.location()).length();

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
            Vec3 offset = normal * 1e-3;  // Small offset to add to location
            Ray reflectionRay(closestHit.location() + offset, reflectionDir);
            reflectionColor = color(reflectionRay, depth + 1, closestShape);
        }

        // Calculate the refraction
        Vec3 refractionColor = Vec3(0, 0, 0);
        if (closestHit.material()->isrefractive) {
            Vec3 normal = closestHit.normal().make_normalised();
            Vec3 ray_dir = (closestHit.location() - r.getOrigin()).make_normalised();
            double n1 = 1.f;
            double n2 = closestHit.material()->refractiveindex;
            if(r.isInside()){
                std::swap(n1,n2);
                normal = -normal;
            }
            double cosi = dot(-normal, ray_dir);
            double n = n1 / n2;
            double k = 1.f - n * n * (1.f - cosi * cosi);
            if(k >= 0.f){
                // The refraction
                Vec3 refractionDir = (n * -ray_dir + (n * cosi - std::sqrt(k)) * normal).make_normalised();
                Ray refract_ray(closestHit.location(), refractionDir, !r.isInside());
                refractionColor = color(refract_ray, depth + 1, closestShape);
            }else{
                // The internal reflection
                Vec3 reflect_dir = (2.f * dot(normal, ray_dir) * normal - ray_dir).make_normalised();
                Ray reflect_ray(closestHit.location(), reflect_dir, r.isInside());
                refractionColor = color(reflect_ray, depth+1, closestShape);
            }
        }

        // Calculate Phong shading
        Vec3 colorOut = diffuse * closestHit.material()->kd +
                        specular * closestHit.material()->ks +
                        ambient * ka;

        if (closestHit.material()->isrefractive) {
            // Only refractive
            colorOut = refractionColor;
        }

        if (closestHit.material()->isreflective) {
            // Only reflective
            colorOut = colorOut * (1 - closestHit.material()->reflectivity) + reflectionColor * closestHit.material()->reflectivity;
        }


        return colorOut;
    }
    else if (renderMode == PATHTRACER){
        /* code */
    }

    //no rendering option selected or no hit
    return scene.getBackgroundColor();
}

void RayTracer::parseRenderWrite(const std::string& filename, RenderMode renderMode, int samplesPP, int maxDepth) {
    std::string inputFilename = "imports/" + filename;
    
    printf("parsing\n");
    parseRender(inputFilename, *this);

    printf("setting data\n");
    this->setRenderMode(renderMode);
    this->setSamplesPP(samplesPP);
    this->setMaxDepth(maxDepth);

    printf("rendering\n");
    this->RenderScene();

    printf("writing\n");
    std::string outputFilename = filename.substr(0, filename.find_last_of(".")) + ".ppm";
    ImageWriter::writePPM(outputFilename, this->RenderScene(), this->camera.getWidth(), this->camera.getHeight());
    //print the name of the image just written
    printf("%s\n", filename.c_str());
    }

int main() {

    RayTracer renderer;
    renderer.parseRenderWrite("binary_primitves.json", BINARY, 1, 4);

    renderer.parseRenderWrite("experiments.json", PHONG, 2, 8);
    renderer.parseRenderWrite("scene.json", PHONG, 1, 8);
    renderer.parseRenderWrite("simple_phong.json", PHONG, 1, 4);
    renderer.parseRenderWrite("mirror_image.json", PHONG, 2, 4);

    return 0;
}