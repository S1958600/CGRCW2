#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Camera.h" // Include your Camera class header
#include "Light.h" // Include your Light class header
#include "Shape.h" // Include your Shape class header

class Scene {
public:
    Scene();

    void setNBounces(int nbounces);
    void setRenderMode(const std::string& rendermode);
    void setCamera(const Camera& camera);
    void setBackgroundColor(const Vec3& color);
    //void addLight(const Light& light);
    void addShape(const Shape& shape);


    int getNBounces() const;
    std::string getRenderMode() const;
    Camera getCamera() const;
    Vec3 getBackgroundColor() const;
    //Light getLight(int index) const;
    Shape getShape(int index) const;

    // Add other methods for managing the scene

private:
    int nbounces_;
    std::string rendermode_;
    Camera camera_;
    Vec3 backgroundcolor_;
    //std::vector<Light> lights_;
    std::vector<Shape> shapes_;
};

#endif // SCENE_H
