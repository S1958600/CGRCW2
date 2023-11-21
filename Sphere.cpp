#include "Sphere.h"
#include "RayStructures.h"
#include <cmath>

// Constructor definition
Sphere::Sphere(const Vec3& center, double radius, const Material& material)
    : Shape(material), center_(center), radius_(radius) {}

// Destructor definition
Sphere::~Sphere() {}

// Method for ray-sphere intersection testing
bool Sphere::intersect(const Ray& ray, Hit& hit) const {
    // Ray-sphere intersection algorithm
    Vec3 oc = ray.getOrigin() - center_;
    
    double a = ray.getDirection().length_squared();
    double half_b = dot(oc, ray.getDirection());
    double c = oc.length_squared() - radius_ * radius_;
    double discriminant = half_b * half_b - a * c;

    // If the discriminant is non-negative, there is an intersection
    if (discriminant >= 0) {
        // Compute the parameter t at which the intersection occurs
        double root = std::sqrt(discriminant);
        double t1 = (-half_b - root) / a;
        double t2 = (-half_b + root) / a;

        // Choose the smallest positive t value
        double t = (t1 > 0) ? t1 : t2;

        // Calculate intersection point and normal
        Vec3 intersectionPoint = ray.pointAtParameter(t);
        Vec3 outward_normal = (intersectionPoint - center_) / radius_;

        // Set Hit information
        if(isCloser(t, hit)){
            hit = Hit(true, t, intersectionPoint, outward_normal);
            return true;
        }
        
    }

    // No intersection
    return false;
}
