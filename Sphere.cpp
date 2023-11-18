#include "Sphere.h"
#include "RayStructures.h"
#include <cmath>

// Constructor definition
Sphere::Sphere(const Vec3& center, float radius, const Material& material)
    : Shape(material), center_(center), radius_(radius) {}

// Destructor definition
Sphere::~Sphere() {}

// Method for ray-sphere intersection testing
bool Sphere::intersect(const Ray& ray, Hit& hit) const {
    // Ray-sphere intersection algorithm
    Vec3 oc = ray.getOrigin() - center_;
    
    float a = ray.getDirection().length_squared();
    float half_b = dot(oc, ray.getDirection());
    float c = oc.length_squared() - radius_ * radius_;
    float discriminant = half_b * half_b - a * c;

    // If the discriminant is non-negative, there is an intersection
    if (discriminant >= 0) {
        // Compute the parameter t at which the intersection occurs
        float root = std::sqrt(discriminant);
        float t1 = (-half_b - root) / a;
        float t2 = (-half_b + root) / a;

        // Choose the smallest positive t value
        float t = (t1 > 0) ? t1 : t2;

        // Calculate intersection point and normal
        Vec3 intersectionPoint = ray.at(t);
        Vec3 outward_normal = (intersectionPoint - center_) / radius_;

        // Set Hit information
        hit = Hit(true, t, outward_normal);

        return true;
    }

    // No intersection
    return false;
}
