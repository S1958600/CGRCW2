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
#include "Material.h"



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
        Vec3 pValue = closestHit.location();
        Vec3 diffuceBaseColor = closestHit.material()->getDiffuseColor(closestHit.u, closestHit.v, pValue);
        Vec3 ambient = Vec3(0.3,0.3,0.3) + (diffuceBaseColor * 0.3);
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
                diffuse += diffuceBaseColor * light->getIntensity() * std::max(diffuseFactor, 0.0);
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
    parseRender(inputFilename, *this);

    
    // Set the texture of the first shape in the scene
    if (!this->scene.getShapes().empty()) {
        Shape* firstShape = this->scene.getShapes()[0];
        printf("shape found\n");
        Material& material = firstShape->getMaterial();
        printf("setting texture\n");
        material.setTexture(new checkered(0.3, Vec3(0.1, 0.1, 0.1), Vec3(1, 0.8, 1)));
    }
    
    this->setRenderMode(renderMode);
    this->setSamplesPP(samplesPP);
    this->setMaxDepth(maxDepth);
    this->RenderScene();
    std::string outputFilename = filename.substr(0, filename.find_last_of(".")) + ".ppm";
    ImageWriter::writePPM(outputFilename, this->RenderScene(), this->camera.getWidth(), this->camera.getHeight());
    //print the name of the image just written
    printf("written %s\n", filename.c_str());
}

int main() {

    
    Camera camera = Camera(1200, 800, Vec3(0, 0, 0), Vec3(0, 0, 1), Vec3(0, 1, 0), 60, 1);

    //materials
    Material matRed = Material(0.3, 0.7, 50, Vec3(0.9, 0.5, 0.5), Vec3(1, 1, 1), false, 0.5, false, 0);
    Material matGreen = Material(0.6, 0.4, 120, Vec3(0.5, 0.9, 0.5), Vec3(1, 1, 1), false, 0.5, false, 0);
    Material matBlue = Material(0.5, 0.5, 80, Vec3(0.5, 0.5, 0.9), Vec3(1, 1, 1), false, 0.5, false, 0);
    Material matWhite = Material(0.7, 0.3, 150, Vec3(0.9, 0.9, 0.9), Vec3(1, 1, 1), false, 0.5, false, 0);
    Material matBlack = Material(0.4, 0.6, 60, Vec3(0.1, 0.1, 0.1), Vec3(1, 1, 1), false, 0.5, false, 0);
    Material matPink = Material(0.6, 0.4, 100, Vec3(0.9, 0.5, 0.9), Vec3(1, 1, 1), false, 0.5, false, 0);
    Material matYellow = Material(0.5, 0.5, 70, Vec3(0.9, 0.9, 0.5), Vec3(1, 1, 1), false, 0.5, false, 0);
    Material matOrange = Material(0.4, 0.6, 90, Vec3(0.9, 0.7, 0.5), Vec3(1, 1, 1), false, 0.5, false, 0);
    Material matMirror = Material(0.5, 0.5, 100, Vec3(0.9, 0.9, 0.9), Vec3(1, 1, 1), true, 1, false, 0);
    Material matGlass = Material(0.5, 0.5, 100, Vec3(0.9, 0.9, 0.9), Vec3(1, 1, 1), false, 0, true, 1.5);
    Material matDiamond = Material(0.5, 0.5, 100, Vec3(0.9, 0.9, 0.9), Vec3(1, 1, 1), false, 0, true, 2.4);
    Material matCheckered = Material(0.5, 0.5, 100, Vec3(0.9, 0.9, 0.9), Vec3(1, 1, 1), false, 0, false, 0);
    matCheckered.setTexture(new checkered(2.5, Vec3(0.8, 0.8, 1), Vec3(1, 0.8, 1)));

    //scene for demonstrating properties
    Scene demoScene = Scene();
    demoScene.setBackgroundColor(Vec3(0.7, 0.5, 0.7));
    //demoScene.addLight(new Light(Vec3(5, 8, 2), Vec3(1, 1, 1)));
    //demoScene.addLight(new Light(Vec3(-5, 8, 1), Vec3(1, 1, 1)));

    demoScene.setBackgroundColor(Vec3(0.7, 0.5, 0.7));
    demoScene.addLight(new Light(Vec3(5, 8, 2), Vec3(1, 0, 0)));
    demoScene.addLight(new Light(Vec3(5.1, 8.1, 2), Vec3(0, 1, 0)));
    demoScene.addLight(new Light(Vec3(5.2, 8.2, 2), Vec3(0, 0, 1)));

    demoScene.addLight(new Light(Vec3(-5, 8, 1), Vec3(0, 0, 1)));
    demoScene.addLight(new Light(Vec3(-5.1, 8.1, 1), Vec3(0, 1, 0)));
    demoScene.addLight(new Light(Vec3(-5.2, 8.2, 1), Vec3(1, 0, 0)));

    //floor
    demoScene.addShape(new Triangle(Vec3(0,-3,-1), Vec3(-30, -3, 30), Vec3(30, -3, 30), matPink));

    //focus of the scene
    demoScene.addShape(new Sphere(Vec3(0, 0, 8), 1, matRed));
    demoScene.addShape(new Sphere(Vec3(2.5, 0, 8), 1, matGreen));
    demoScene.addShape(new Sphere(Vec3(-2.5, 0, 8), 1, matBlue));

    demoScene.addShape(new Cylinder(Vec3(0, 3, 10), Vec3(0,0,1), 0.8, 1, matPink));
    demoScene.addShape(new Cylinder(Vec3(2.5, 3, 10), Vec3(0,1,0), 0.8, 1, matRed));
    demoScene.addShape(new Cylinder(Vec3(-2.5, 3, 10), Vec3(1,0,0), 0.8, 1, matOrange));

    demoScene.addShape(new Triangle(Vec3(-1.5, -3, 6), Vec3(-2.5, -3, 6), Vec3(-2, -2, 8.1), matBlue));
    demoScene.addShape(new Triangle(Vec3(2.5, -3, 6), Vec3(1.5, -3, 6), Vec3(2, -2, 8.1), matGreen));   

    //'sun'
    demoScene.addShape(new Sphere(Vec3(0, 5, 45), 20, matYellow));

    RayTracer renderer;
    renderer.setScene(demoScene);
    renderer.setCamera(camera);
    renderer.setRenderMode(PHONG);
    renderer.setSamplesPP(4);

    //ImageWriter::writePPM("demo.ppm", renderer.RenderScene(), camera.getWidth(), camera.getHeight());



    //scene for demonstrating video
    Scene videoScene = Scene();

    videoScene.setBackgroundColor(Vec3(0.7, 0.5, 0.7));
    videoScene.addLight(new Light(Vec3(10, 8, 10), Vec3(1, 0, 0)));
    videoScene.addLight(new Light(Vec3(10.1, 8, 10.1), Vec3(0, 1, 0)));
    videoScene.addLight(new Light(Vec3(10.2, 8, 10.2), Vec3(0, 0, 1)));

    videoScene.addLight(new Light(Vec3(-10, 8, -10), Vec3(0, 0, 1)));
    videoScene.addLight(new Light(Vec3(-10.1, 8, -10.1), Vec3(0, 1, 0)));
    videoScene.addLight(new Light(Vec3(-10.2, 8, -10.2), Vec3(1, 0, 0)));

    //floor
    videoScene.addShape(new Triangle(Vec3(-30,0,-30), Vec3(-30, 0, 30), Vec3(30, 0, 30), matCheckered));
    videoScene.addShape(new Triangle(Vec3(-30,0,-30), Vec3(30,0, 30), Vec3(30, 0, -30), matCheckered));

    //marbles on the floor
    videoScene.addShape(new Sphere(Vec3(-20, 0.5, 2.5), 0.5, matRed));
    videoScene.addShape(new Sphere(Vec3(-20, 1, 14), 1, matRed));
    videoScene.addShape(new Sphere(Vec3(-16, 1.5, -5), 1.5, matGreen));
    videoScene.addShape(new Sphere(Vec3(-14, 1.5, 12), 1.5, matBlue));
    videoScene.addShape(new Sphere(Vec3(-11, 0.5, -8), 0.5, matPink));
    videoScene.addShape(new Sphere(Vec3(-10, 1, 16), 1, matGreen));
    videoScene.addShape(new Sphere(Vec3(-6, 1.5, -12), 1.5, matWhite));
    videoScene.addShape(new Sphere(Vec3(-4, 0.5, 3), 0.5, matMirror));
    videoScene.addShape(new Sphere(Vec3(-4, 1.5, -13), 1.5, matOrange));
    videoScene.addShape(new Sphere(Vec3(3, 0.5, 12), 0.5, matBlue));
    videoScene.addShape(new Sphere(Vec3(5, 0.5, -16), 0.5, matBlack));
    videoScene.addShape(new Sphere(Vec3(9, 1, 4), 1, matOrange));
    videoScene.addShape(new Sphere(Vec3(9, 1.5, -9), 1.5, matYellow));
    videoScene.addShape(new Sphere(Vec3(12, 2, 4), 2, matGlass));
    videoScene.addShape(new Sphere(Vec3(-15, 2.3, -10), 2.3, matDiamond));
    videoScene.addShape(new Sphere(Vec3(17, 2, -8), 2, matGlass));
    videoScene.addShape(new Sphere(Vec3(19, 1, 8), 1, matPink));


    videoScene.addShape(new Cylinder(Vec3(0, 1.5, 0), Vec3(0,1,0), 2, 3, matMirror)); // Bottom right corner


    Camera videoCamera = Camera(1200, 800, Vec3(0, 10, -20), Vec3(0, -1, 1), Vec3(0, 1, 0), 60, 1);

    renderer.setScene(videoScene);
    renderer.setCamera(videoCamera);
    renderer.setSamplesPP(1);

    ImageWriter::writePPM("video.ppm", renderer.RenderScene(), camera.getWidth(), camera.getHeight());

    // Number of frames
    int numFrames = 100;

    // Radius of the camera's circular path
    double radius = 20;

    // Angle step per frame
    double angleStep = 2 * M_PI / numFrames;

    for (int i = 0; i < numFrames; i++) {
        // Calculate the camera's position on the circular path
        double angle = i * angleStep;
        Vec3 cameraPosition(radius * cos(angle), 10, radius * sin(angle));

        // Create a camera that looks at the origin
        Camera videoCamera = Camera(1200, 800, cameraPosition, Vec3(0, 0, 0), Vec3(0, 1, 0), 45, 1);

        // Set the scene and camera
        renderer.setScene(videoScene);
        renderer.setCamera(videoCamera);
        renderer.setSamplesPP(3);

        // Render the scene and write it to a file
        std::string filename = "outputs/frame" + std::to_string(i) + ".ppm";
        ImageWriter::writePPM(filename, renderer.RenderScene(), camera.getWidth(), camera.getHeight());
    }

    /*
    renderer.parseRenderWrite("experiments.json", PHONG, 2, 8);
    renderer.parseRenderWrite("binary_primitves.json", BINARY, 1, 4);
    renderer.parseRenderWrite("scene.json", PHONG, 1, 8);
    renderer.parseRenderWrite("simple_phong.json", PHONG, 1, 4);
    renderer.parseRenderWrite("mirror_image.json", PHONG, 2, 4);

    */
    return 0;
}