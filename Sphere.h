#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "RayStructures.h"

class Sphere : public Shape {
public:
    Sphere(const Vec3 &center, double radius, const Material &material);
    virtual ~Sphere();

    void calculateBoundingBox();

    Vec3 getCenter() const;

    void setUVCoordinates(const Vec3& intersectionPoint, Hit& hit) const;


    // Method for ray-sphere intersection testing
    virtual bool intersect(const Ray& ray, Hit& hit, Shape* ignoreShape) const override;

private:
    Vec3 center_;  // Sphere center
    double radius_;               // Sphere radius
};

#endif // SPHERE_H
