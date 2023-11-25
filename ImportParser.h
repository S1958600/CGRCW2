#ifndef IMPORTPARSER_H
#define IMPORTPARSER_H

#include <iostream>
#include <fstream>

#include "json.hpp"
#include "Scene.h"
#include "Camera.h"
#include "RayTracer.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Triangle.h"

using json = nlohmann::json;

RayTracer parseRender(const std::string& filename) {
    std::ifstream file(filename);
    json jsonData;
    file >> jsonData;

    RayTracer renderer;
    Scene scene;

    // Parsing camera data
    //printf("Extracting camera\n");
    json cameraData = jsonData["camera"];
    //printf("Extracting camera width\n");
    int width = cameraData["width"];
    //printf("Extracting camera height\n");
    int height = cameraData["height"];
    //printf("Extracting camera position\n");
    Vec3 position(cameraData["position"][0], cameraData["position"][1], cameraData["position"][2]);
    //printf("Extracting camera lookat\n");
    Vec3 lookAt(cameraData["lookAt"][0], cameraData["lookAt"][1], cameraData["lookAt"][2]);
    //printf("Extracting camera upvector\n");
    Vec3 upVector(cameraData["upVector"][0], cameraData["upVector"][1], cameraData["upVector"][2]);
    //printf("Extracting camera fov\n");
    double fov = cameraData["fov"];
    //printf("Extracting camera exposure\n");
    double exposure = cameraData["exposure"];

    // Creating and setting the Camera object
    Camera camera(width, height, position, lookAt, upVector, fov, exposure);

    //printf("Camera created\n");

    // Parsing scene data
    json sceneData = jsonData["scene"];
    
    // Extract background color
    //printf("Extracting background color\n");
    std::vector<double> bgColor = sceneData["backgroundcolor"];
    scene.setBackgroundColor(Vec3(bgColor[0], bgColor[1], bgColor[2]));


    // Extract light sources
    for (auto& light : sceneData["lightsources"]) {
        // Parse each light source and add it to the scene
        Vec3 position(light["position"][0], light["position"][1], light["position"][2]);
        Vec3 intensity(light["intensity"][0], light["intensity"][1], light["intensity"][2]);

        scene.addLight(new Light(position, intensity));
        //printf("Light added\n");
    }

    //printf("Lights parsed\n");

    // Extract shapes
    for (auto& shape : sceneData["shapes"]) {
        //printf("Extracting material\n");
        Material mat;

        if (shape.contains("material")) {

            //printf("Material found\n");
            mat = Material(shape["material"]["ks"],
                shape["material"]["kd"], shape["material"]["specularexponent"],
                Vec3(shape["material"]["diffusecolor"][0], shape["material"]["diffusecolor"][1], shape["material"]["diffusecolor"][2]),
                Vec3(shape["material"]["specularcolor"][0], shape["material"]["specularcolor"][1], shape["material"]["specularcolor"][2]),
                shape["material"]["isreflective"], shape["material"]["reflectivity"],
                shape["material"]["isrefractive"], shape["material"]["refractiveindex"]);

                //printf("Adding material, %f %f %f \n", shape["material"]["diffusecolor"][0].get<double>(), shape["material"]["diffusecolor"][1].get<double>(), shape["material"]["diffusecolor"][2].get<double>());

        } else {
            //printf("No material\n");
            mat = Material();
        }

        if (shape["type"] == "sphere") {
            //printf("Extracting sphere\n");
            scene.addShape(new Sphere(Vec3(shape["center"][0], 
            shape["center"][1], shape["center"][2]), shape["radius"], mat));
        }
        else if (shape["type"] == "cylinder") {
            //printf("Extracting cylinder\n");
            scene.addShape(new Cylinder(Vec3(shape["center"][0], shape["center"][1], shape["center"][2]),
            Vec3(shape["axis"][0], shape["axis"][1], shape["axis"][2]), shape["radius"], shape["height"], mat));
        }
        else if (shape["type"] == "triangle") {
            //printf("Extracting triangle\n");
            scene.addShape(new Triangle(Vec3(shape["v0"][0], shape["v0"][1], shape["v0"][2]),
            Vec3(shape["v1"][0], shape["v1"][1], shape["v1"][2]), 
            Vec3(shape["v2"][0], shape["v2"][1], shape["v2"][2]), mat));
        }
        else {
            std::cout << "Error: Unknown shape type: " << shape["type"] << std::endl;
        }
    }

    //printf("Shapes parsed\n");

    RenderMode renderMode;
    if (jsonData["rendermode"] == "binary") {
        //printf("Extracting binary render mode\n");
        renderMode = BINARY;
    }
    else if (jsonData["rendermode"] == "phong") {
        //printf("Extracting phong render mode\n");
        renderMode = PHONG;
    }
    else if (jsonData["rendermode"] == "pathtracer") {
        //printf("Extracting pathtracer render mode\n");
        renderMode = PATHTRACER;
    }
    else {
        std::cout << "Error: Unknown render mode: " << jsonData["rendermode"] << std::endl;
    }


    if (jsonData.contains("nbounces")) {
        //printf("Extracting number of bounces\n");
        renderer = RayTracer(renderMode, camera, scene, jsonData["nbounces"]);
    } else {
        //printf("Extracting without bounces\n");
        renderer = RayTracer(renderMode, camera, scene);
    }
    return renderer;
}


void parseRender(const std::string& filename, RayTracer& renderer) {
    std::ifstream file(filename);
    json jsonData;
    file >> jsonData;

    Scene scene;

    json cameraData = jsonData["camera"];
    int width = cameraData["width"];
    int height = cameraData["height"];
    Vec3 position(cameraData["position"][0], cameraData["position"][1], cameraData["position"][2]);
    Vec3 lookAt(cameraData["lookAt"][0], cameraData["lookAt"][1], cameraData["lookAt"][2]);
    Vec3 upVector(cameraData["upVector"][0], cameraData["upVector"][1], cameraData["upVector"][2]);
    double fov = cameraData["fov"];
    double exposure = cameraData["exposure"];

    // Creating and setting the Camera object
    Camera camera(width, height, position, lookAt, upVector, fov, exposure);

    // Parsing scene data
    json sceneData = jsonData["scene"];
    
    std::vector<double> bgColor = sceneData["backgroundcolor"];
    scene.setBackgroundColor(Vec3(bgColor[0], bgColor[1], bgColor[2]));

    // Extract light sources
    for (auto& light : sceneData["lightsources"]) {
        Vec3 position(light["position"][0], light["position"][1], light["position"][2]);
        Vec3 intensity(light["intensity"][0], light["intensity"][1], light["intensity"][2]);
        scene.addLight(new Light(position, intensity));
    }
    // Extract shapes
    for (auto& shape : sceneData["shapes"]) {
        Material mat;
        if (shape.contains("material")) {
            mat = Material(shape["material"]["ks"],
                shape["material"]["kd"], shape["material"]["specularexponent"],
                Vec3(shape["material"]["diffusecolor"][0], shape["material"]["diffusecolor"][1], shape["material"]["diffusecolor"][2]),
                Vec3(shape["material"]["specularcolor"][0], shape["material"]["specularcolor"][1], shape["material"]["specularcolor"][2]),
                shape["material"]["isreflective"], shape["material"]["reflectivity"],
                shape["material"]["isrefractive"], shape["material"]["refractiveindex"]);
        } else {
            mat = Material();
            //no material
        }

        if (shape["type"] == "sphere") {
            scene.addShape(new Sphere(Vec3(shape["center"][0], 
            shape["center"][1], shape["center"][2]), shape["radius"], mat));
        }
        else if (shape["type"] == "cylinder") {
            scene.addShape(new Cylinder(Vec3(shape["center"][0], shape["center"][1], shape["center"][2]),
            Vec3(shape["axis"][0], shape["axis"][1], shape["axis"][2]), shape["radius"], shape["height"], mat));
        }
        else if (shape["type"] == "triangle") {
            scene.addShape(new Triangle(Vec3(shape["v0"][0], shape["v0"][1], shape["v0"][2]),
            Vec3(shape["v1"][0], shape["v1"][1], shape["v1"][2]), 
            Vec3(shape["v2"][0], shape["v2"][1], shape["v2"][2]), mat));
        }
        else {
            std::cout << "Error: Unknown shape type: " << shape["type"] << std::endl;
        }
    }

    RenderMode renderMode;
    if (jsonData["rendermode"] == "binary") {renderMode = BINARY;}
    else if (jsonData["rendermode"] == "phong") {renderMode = PHONG;}
    else if (jsonData["rendermode"] == "pathtracer") {renderMode = PATHTRACER;}
    else {std::cout << "Error: Unknown render mode: " << jsonData["rendermode"] << std::endl;}

    renderer.setRenderMode(renderMode);
    renderer.setCamera(camera);
    renderer.setScene(scene);
}


#endif // IMPORTPARSER_H