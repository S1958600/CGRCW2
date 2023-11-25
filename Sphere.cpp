#include "Sphere.h"
#include "RayStructures.h"
#include <cmath>

// Constructor definition
Sphere::Sphere(const Vec3& center, double radius, const Material& material)
    : Shape(material), center_(center), radius_(radius) {}

// Destructor definition
Sphere::~Sphere() {}

// Method for ray-sphere intersection testing
bool Sphere::intersect(const Ray& ray, Hit& hit, Shape* ignoreShape) const {
    if(ignoreShape == this){return false;}

    Vec3 vectorToCenter = center_ - ray.getOrigin();
    double projectionLength = dot(vectorToCenter, ray.getDirection());
    if(projectionLength < 0){
        return false;
    }

    double distanceSquared = dot(vectorToCenter, vectorToCenter) - projectionLength * projectionLength;
    if(distanceSquared < 0 || distanceSquared > radius_ * radius_){
        return false;
    }

    double halfChordLength = std::sqrt(radius_ * radius_ - distanceSquared);
    double t0 = projectionLength - halfChordLength;
    double t1 = projectionLength + halfChordLength;

    double intersectionDistance;
    if(t0 < 1e-4 && t1 < 1e-4){
        return false;
    }else if(t0 < 1e-4){
        intersectionDistance = t1;
    }else if(t1 < 1e-4){
        intersectionDistance = t0;
    }else{
        intersectionDistance = std::min(t0, t1);
    }

    Vec3 intersectionPoint = ray.getOrigin() + ray.getDirection() * intersectionDistance;
    Vec3 outwardNormal = (intersectionPoint - center_).make_normalised();

    // Move the intersection point slightly along the ray direction
    intersectionPoint = intersectionPoint + ray.getDirection() * 1e-4;

    if(isCloser(intersectionDistance, hit)){
        hit = Hit(true, intersectionDistance, intersectionPoint, outwardNormal, &material_);
        return true;
    }

    return false;
}