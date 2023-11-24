#ifndef SPHERE_H
#define SPHERE_H

#include "Shape.h"
#include "RayStructures.h"

class Sphere : public Shape {
public:
    // Constructor
    //Sphere(const Vec3 center, double radius, const Material& material);

    Sphere(const Vec3 &center, double radius, const Material &material);
    
    // Destructor
    virtual ~Sphere();

    // Method for ray-sphere intersection testing
    virtual bool intersect(const Ray& ray, Hit& hit, Shape* ignoreShape) const override;

private:
    Vec3 center_;  // Sphere center
    double radius_;               // Sphere radius
};

#endif // SPHERE_H
