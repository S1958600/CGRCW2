#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "RayStructures.h"

class Sphere : public Shape {
public:
    // Constructor
    //Sphere(const Vec3 center, float radius, const Material& material);

    Sphere(const Vec3 &center, float radius, const Material &material);

    // Destructor
    virtual ~Sphere();

    // Method for ray-sphere intersection testing
    virtual bool intersect(const Ray& ray, Hit& hit) const override;

private:
    Vec3 center_;  // Sphere center
    float radius_;               // Sphere radius
};

#endif // SPHERE_H
