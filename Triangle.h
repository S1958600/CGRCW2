#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"
#include "RayStructures.h"

class Triangle : public Shape {
public:
    Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Material& material);
    ~Triangle();

    void calculateBoundingBox();

    Vec3 getCenter() const;

    bool intersect(const Ray& ray, Hit& hit, Shape* ignoreShape) const override;

private:
    Vec3 v0_;
    Vec3 v1_;
    Vec3 v2_;
    Material material_;
};

#endif // TRIANGLE_H
