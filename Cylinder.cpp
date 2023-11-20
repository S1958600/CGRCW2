#include "Cylinder.h"
#include <cmath>

Cylinder::Cylinder(const Vec3& center, const Vec3& axis, float radius, float height, const Material& material)
    : Shape(material), center_(center), axis_(axis), radius_(radius), height_(height), material_(material) {}

Cylinder::~Cylinder() {}


bool Cylinder::withinCylinderBounds(float t, const Vec3& ray_origin, const Vec3& ray_direction) const {
    Vec3 hit_point = ray_origin + t * ray_direction;
    Vec3 distance = hit_point - center_;

    // Calculate the projection of the distance vector onto the axis of the cylinder
    float projection = dot(distance, axis_);

    // Check if the hit point is within the bounds of the cylinder
    if (projection >= 0 && projection <= height_) {
        // Calculate the perpendicular distance from the hit point to the axis
        Vec3 perpendicular_distance = distance - projection * axis_;

        // Check if the perpendicular distance is within the radius
        if (perpendicular_distance.length_squared() <= radius_ * radius_) {
            return true; // Inside the cylinder bounds
        }
    }

    return false; // Outside the cylinder bounds
}


bool Cylinder::intersect(const Ray& ray, Hit& hit) const {
    Vec3 oc = ray.getOrigin() - center_;

    float a = dot(ray.getDirection() - axis_ * dot(ray.getDirection(), axis_), ray.getDirection() - axis_ * dot(ray.getDirection(), axis_));
    float b = 2 * dot(ray.getDirection() - axis_ * dot(ray.getDirection(), axis_), oc - axis_ * dot(oc, axis_));
    float c = dot(oc - axis_ * dot(oc, axis_), oc - axis_ * dot(oc, axis_)) - radius_ * radius_;

    float discriminant = b * b - 4 * a * c;

    if (discriminant >= 0) {
        float root = std::sqrt(discriminant);
        float t1 = (-b - root) / (2 * a);
        float t2 = (-b + root) / (2 * a);

        // Check both roots and choose the one that lies within the bounds of the cylinder
        float t = std::numeric_limits<float>::max();
        Vec3 intersectionPoint;
        Vec3 outward_normal;

        for (float root : {t1, t2}) {
            Vec3 potentialIntersectionPoint = ray.pointAtParameter(root);
            Vec3 distance = potentialIntersectionPoint - center_;

            // Check if the intersection point is within the lateral surface bounds
            float projection = dot(distance, axis_);
            if (projection >= 0 && projection <= height_) {
                Vec3 perpendicular_distance = distance - projection * axis_;
                if (perpendicular_distance.length_squared() <= radius_ * radius_) {
                    if (root < t) {
                        t = root;
                        intersectionPoint = potentialIntersectionPoint;
                        outward_normal = (intersectionPoint - center_ - axis_ * projection) / radius_;
                    }
                }
            }
        }

        if (t != std::numeric_limits<float>::max()) {
            // Add an epsilon offset to the intersection point
            float epsilon = 0.0001f;
            intersectionPoint += epsilon * outward_normal;
            hit = Hit(true, t, intersectionPoint, outward_normal);
            return true;
        }

        // Check for intersections with end caps
        float t_top_cap = dot((center_ + height_ * axis_ - ray.getOrigin()), axis_) / dot(ray.getDirection(), axis_);
        float t_bottom_cap = dot((center_ - ray.getOrigin()), axis_) / dot(ray.getDirection(), axis_);

        // Check if the intersection point is within the end cap bounds
        Vec3 top_cap_intersection = ray.pointAtParameter(t_top_cap);
        Vec3 bottom_cap_intersection = ray.pointAtParameter(t_bottom_cap);

        if ((top_cap_intersection - center_ - height_ * axis_).length_squared() <= radius_ * radius_) {
            Vec3 outward_normal = axis_;
            hit = Hit(true, t_top_cap, top_cap_intersection, outward_normal);
            return true;
        }

        if ((bottom_cap_intersection - center_).length_squared() <= radius_ * radius_) {
            Vec3 outward_normal = -axis_;
            hit = Hit(true, t_bottom_cap, bottom_cap_intersection, outward_normal);
            return true;
        }

    }

    return false;
}



