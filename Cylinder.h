#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"
#include "RayStructures.h"

class Cylinder : public Shape {
public:

    Cylinder(const Vec3& center, const Vec3& axis, double radius, double height, const Material& material);

    ~Cylinder();

    void calculateBoundingBox();

    Vec3 getCenter() const;

    void setUVCoordinates(const Vec3& intersectionPoint, Hit& hit) const;

    
    bool intersect(const Ray& ray, Hit& hit, Shape* ignoreShape) const;

private:
    Vec3 center_;
    Vec3 axis_;
    double radius_;
    double height_;
    Material material_;
};

#endif // CYLINDER_H
