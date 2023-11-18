#include <iostream>
#include <fstream>
#include "json.hpp"
#include "Scene.h"

using json = nlohmann::json;

Scene parseJSON(const std::string& filename) {
    std::ifstream file(filename);
    json jsonData;
    file >> jsonData;

    Scene scene;

    // Extracting data from JSON and populating the Scene object

    scene.setNBounces(jsonData["nbounces"]);
    scene.setRenderMode(jsonData["rendermode"]);

    // Parsing camera data
    json cameraData = jsonData["camera"];
    int width = cameraData["width"];
    int height = cameraData["height"];
    Vec3 position(cameraData["position"][0], cameraData["position"][1], cameraData["position"][2]);
    Vec3 lookAt(cameraData["lookAt"][0], cameraData["lookAt"][1], cameraData["lookAt"][2]);
    Vec3 upVector(cameraData["upVector"][0], cameraData["upVector"][1], cameraData["upVector"][2]);
    float fov = cameraData["fov"];
    float exposure = cameraData["exposure"];

    // Creating and setting the Camera object
    Camera camera(width, height, position, lookAt, upVector, fov, exposure);
    scene.setCamera(camera);

    // Parsing scene data
    json sceneData = jsonData["scene"];
    
    // Extract background color
    std::vector<float> bgColor = sceneData["backgroundcolor"];
    scene.setBackgroundColor(Vec3(bgColor[0], bgColor[1], bgColor[2]));

    // Extract light sources
    for (auto& light : sceneData["lightsources"]) {
        // Parse each light source and add it to the scene
        // ...
    }

    // Extract shapes
    for (auto& shape : sceneData["shapes"]) {
        // Parse each shape and add it to the scene
        // ...
    }

    return scene;
}


/*
int main() {
    Scene scene = parseJSON("scene.json");

    // Use the parsed scene for rendering or further processing
    // ...

    return 0;
}
*/
