#include "Scene.h"

Scene::Scene() : backgroundcolor_(Vec3(0.0, 0.0, 0.0)) {}

void Scene::setBackgroundColor(const Vec3& color) {
    backgroundcolor_ = color;
}

Shape* Scene::getShape(int index) const {
    return shapes_[index];
}

std::vector<Shape*> Scene::getShapes() const {
    return shapes_;
}


/*
void Scene::addLight(const Light& light) {
    lights_.push_back(light);
}
*/


void Scene::addShape(Shape* shape) {
    shapes_.push_back(shape);
}

Vec3 Scene::getBackgroundColor() const {
    return backgroundcolor_;
}


// Implement other methods for managing the scene

// Optionally implement destructor, getters, setters, etc., as required
