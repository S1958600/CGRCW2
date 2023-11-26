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

    void setUVCoordinates(double u, double v, Hit& hit) const;


    bool intersect(const Ray& ray, Hit& hit, Shape* ignoreShape) const override;

    Vec3 uv0_, uv1_, uv2_;  // UV coordinates at vertices


private:
    Vec3 v0_;
    Vec3 v1_;
    Vec3 v2_;
    Material material_;

};

#endif // TRIANGLE_H
