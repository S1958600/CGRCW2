#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Camera.h" // Include your Camera class header
#include "Light.h" // Include your Light class header
#include "Shape.h" // Include your Shape class header

class Scene {
public:
    Scene();

    void setBackgroundColor(const Vec3& color);
    //void addLight(const Light& light);
    void addShape(Shape* shape);
    void addLight(Light* light);


    Vec3 getBackgroundColor() const;
    //Light getLight(int index) const;
    Shape* getShape(int index) const;
    std::vector<Shape*> getShapes() const;
    std::vector<Light*> getLights() const;

private:
    Vec3 backgroundcolor_;
    //std::vector<Light> lights_;
    std::vector<Shape*> shapes_;
    std::vector<Light*> lights_;
};

#endif // SCENE_H
