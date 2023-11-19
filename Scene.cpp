#include "Scene.h"

Scene::Scene() : nbounces_(0), rendermode_("") {
    // Initialize other attributes if needed
}

void Scene::setNBounces(int nbounces) {
    nbounces_ = nbounces;
}

void Scene::setRenderMode(const std::string& rendermode) {
    rendermode_ = rendermode;
}

void Scene::setCamera(const Camera& camera) {
    camera_ = camera;
}

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

int Scene::getNBounces() const {
    return nbounces_;
}

std::string Scene::getRenderMode() const {
    return rendermode_;
}

Camera Scene::getCamera() const {
    return camera_;
}

Vec3 Scene::getBackgroundColor() const {
    return backgroundcolor_;
}


// Implement other methods for managing the scene

// Optionally implement destructor, getters, setters, etc., as required
