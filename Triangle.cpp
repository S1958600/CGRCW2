#include "Triangle.h"
#include "Vec3.h"
#include <cmath>

Triangle::Triangle(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Material& material)
    : Shape(material), v0_(v0), v1_(v1), v2_(v2), material_(material) {
        calculateBoundingBox();
        // Compute the center of the triangle
        Vec3 center = getCenter();

        // Compute default UV coordinates as if the triangle was a plane
        uv0_ = v0 - center;
        uv1_ = v1 - center;
        uv2_ = v2 - center;

        // Set the z-component of the UV coordinates to 0
        uv0_ = Vec3(uv0_.x(), uv0_.y(), 0);
        uv1_ = Vec3(uv1_.x(), uv1_.y(), 0);
        uv2_ = Vec3(uv2_.x(), uv2_.y(), 0);
    }


Triangle::~Triangle() {}

void Triangle::calculateBoundingBox() {
    double offset = 0.0001;  // Define an offset
    bbox.min = Vec3::min(v0_, Vec3::min(v1_, v2_)) - Vec3(offset, offset, offset);  
    // Subtract the offset from the minimum coordinates
    bbox.max = Vec3::max(v0_, Vec3::max(v1_, v2_)) + Vec3(offset, offset, offset);  
    // Add the offset to the maximum coordinates
}

Vec3 Triangle::getCenter() const {
    return (v0_ + v1_ + v2_) / 3.0;
}

void Triangle::setUVCoordinates(double u, double v, Hit& hit) const {
    // Interpolate the texture coordinates using barycentric coordinates
    double w = 1 - u - v;
    double uCoord = u * uv0_.x() + v * uv1_.x() + w * uv2_.x();
    double vCoord = u * uv0_.y() + v * uv1_.y() + w * uv2_.y();

    // Set texture coordinates
    hit.u = uCoord;
    hit.v = vCoord;
}

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
        hit.setShape(const_cast<Triangle*>(this));
        setUVCoordinates(u, v, hit);
        return true;
    }

    return false;
}
