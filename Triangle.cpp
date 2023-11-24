#include "Triangle.h"
#include <cmath>

Triangle::Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Material& material)
    : Shape(material), v0_(v0), v1_(v1), v2_(v2), material_(material) {}


Triangle::~Triangle() {}

bool Triangle::intersect(const Ray& ray, Hit& hit, Shape* ignoreShape) const {
    if (this == ignoreShape) {return false;}

    Vec3 e1 = v1_ - v0_;
    Vec3 e2 = v2_ - v0_;

    Vec3 p = cross(ray.getDirection(), e2);
    double det = dot(e1, p);

    // Check if ray and triangle are parallel
    if (std::fabs(det) < 1e-8) {
        return false;
    }

    double invDet = 1.0f / det;
    Vec3 t = ray.getOrigin() - v0_;

    double u = dot(t, p) * invDet;
    if (u < 0.0f || u > 1.0f) {
        return false;
    }

    Vec3 q = cross(t, e1);
    double v = dot(ray.getDirection(), q) * invDet;
    if (v < 0.0f || u + v > 1.0f) {
        return false;
    }

    double tValue = dot(e2, q) * invDet;
    if (tValue <= 0.0f) {
        return false;
    }

    // Set Hit information
    Vec3 intersectionPoint = ray.pointAtParameter(tValue);
    Vec3 outward_normal = cross(e1, e2).make_normalised();
    
    if (isCloser(tValue, hit)) {
        hit = Hit(true, tValue, intersectionPoint, outward_normal, &material_);
        return true;
    }

    return false;
}
