#include "Cylinder.h"
#include <cmath>

Cylinder::Cylinder(const Vec3& center, const Vec3& axis, double radius, double height, const Material& material)
    : Shape(material), center_(center), axis_(axis), radius_(radius), height_(height), material_(material) {
        calculateBoundingBox();
    }

Cylinder::~Cylinder() {}

void Cylinder::calculateBoundingBox() {
    Vec3 radiusVec(radius_ * 2, radius_ * 2, height_ * 2);
    bbox.min = center_ - radiusVec;
    bbox.max = center_ + radiusVec;
}

Vec3 Cylinder::getCenter() const {
    return center_;
}

void Cylinder::setUVCoordinates(const Vec3& intersectionPoint, Hit& hit) const {
    Vec3 outwardNormal = (intersectionPoint - center_).make_normalised();

    // Compute texture coordinates
    double phi = atan2(outwardNormal.y(), outwardNormal.x());
    double theta = asin(outwardNormal.z());
    double u = 1 - (phi + M_PI) / (2 * M_PI);
    double v = (theta + M_PI_2) / M_PI;

    // Set texture coordinates
    hit.u = u;
    hit.v = v;
}


bool Cylinder::intersect(const Ray& ray, Hit& hit, Shape* ignoreShape) const {
    if (this == ignoreShape) {return false;}

    bool hitSomething = false;
    // Calculate the vector from the ray origin to the cylinder axis
    Vec3 oc = (ray.getOrigin()-axis_) - (center_-axis_);

    // Calculate the coefficients of the quadratic equation
    double a = dot(ray.getDirection() - axis_ * dot(ray.getDirection(), axis_), ray.getDirection() - axis_ * dot(ray.getDirection(), axis_));
    double b = 2 * dot(ray.getDirection() - axis_ * dot(ray.getDirection(), axis_), oc - axis_ * dot(oc, axis_));
    double c = dot(oc - axis_ * dot(oc, axis_), oc - axis_ * dot(oc, axis_)) - radius_ * radius_;

    double discriminant = b * b - 4 * a * c;
    
    // If the discriminant is negative, there are no real roots
    double tolerance = 1e-6f;
    if (discriminant >= -tolerance) {
        double root = std::sqrt(discriminant);
        double t1 = (-b - root) / (2 * a);
        double t2 = (-b + root) / (2 * a);

        // Check both roots and choose the one that is closest to the ray origin
        double t = std::numeric_limits<double>::max();
        Vec3 intersectionPoint;
        Vec3 outward_normal;

        for (double root : {t1, t2}) {
            Vec3 potentialIntersectionPoint = ray.pointAtParameter(root);
            if (root < t && t>0) {
                t = root;
                intersectionPoint = potentialIntersectionPoint;
                outward_normal = (intersectionPoint - center_ - axis_ * dot(intersectionPoint - center_, axis_)) / radius_;
            }
        }

        // Check if the intersection point is within the cylinder bounds
        double z = dot(oc, axis_) + t * dot(ray.getDirection(), axis_);

        if (z >= -height_ && z <= height_ ) {
            // Check if the intersection point is closer than the previous closest hit
            if (isCloser(t, hit)){
                    hit = Hit(true, t, intersectionPoint, outward_normal, &material_);
                    hit.setShape(const_cast<Cylinder*>(this));
                    hitSomething = true;
            }
        }
    }

    // Check for intersections with end caps
    double t_top_cap = dot((center_ + height_ * axis_ - ray.getOrigin()), axis_) / dot(ray.getDirection(), axis_);
    double t_bottom_cap = dot((center_ -  height_ * axis_ - ray.getOrigin()), axis_) / dot(ray.getDirection(), axis_);

    // Check if the intersection point is within the end cap bounds
    Vec3 top_cap_intersection = ray.pointAtParameter(t_top_cap);
    Vec3 bottom_cap_intersection = ray.pointAtParameter(t_bottom_cap);
    if ((top_cap_intersection - center_ - height_ * axis_).length_squared() <= radius_ * radius_) {
        Vec3 outward_normal = axis_;
        if (isCloser(t_top_cap, hit)) {
            hit = Hit(true, t_top_cap, top_cap_intersection, outward_normal, &material_);
            hit.setShape(const_cast<Cylinder*>(this));
            hitSomething = true;
        }
    }

    if ((bottom_cap_intersection - (center_ - height_ * axis_)).length_squared() <= radius_ * radius_) {
        Vec3 outward_normal = -axis_;

        if (isCloser(t_bottom_cap, hit)) {
            hitSomething = true;    
            hit = Hit(true, t_bottom_cap, bottom_cap_intersection, outward_normal, &material_);
            hit.setShape(const_cast<Cylinder*>(this));
        }
    }
    return hitSomething;
}



