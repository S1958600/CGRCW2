#ifndef CYLINDER_H
#define CYLINDER_H

#include "Shape.h"
#include "RayStructures.h"

class Cylinder : public Shape {
public:

    Cylinder(const Vec3& center, const Vec3& axis, float radius, float height, const Material& material);

    ~Cylinder();

    bool withinCylinderBounds(float t, const Vec3& ray_origin, const Vec3& ray_direction) const;

    bool intersect(const Ray& ray, Hit& hit) const;

private:
    Vec3 center_;
    Vec3 axis_;
    float radius_;
    float height_;
    Material material_;
};

#endif // CYLINDER_H
