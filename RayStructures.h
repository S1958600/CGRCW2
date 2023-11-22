#ifndef RAYH
#define RAYH

#include "Vec3.h"
#include "Material.h"

class Hit {
public:
    // Constructors
    Hit() : hitOccurred_(false), t_(0.0f) {}
    Hit(bool hitOccurred, double t, const Vec3& location, const Vec3& normal, const Material* material)
        : hitOccurred_(hitOccurred), t_(t), location_(location), normal_(normal), material_(material) {}

    // Accessor methods
    bool hitOccurred() const { return hitOccurred_; }
    double t() const { return t_; }
    Vec3 location() const { return location_; }
    Vec3 normal() const { return normal_; }
    const Material* material() const { return material_; }


private:
    bool hitOccurred_; // Indicates if a hit occurred
    double t_; // Parameter along the ray where the intersection occurs
    Vec3 location_; // Coordinates of the intersection point
    Vec3 normal_; // Normal at the intersection point
    const Material* material_; // Material of the intersected object

};


class Ray {
    public:

        Ray() {}
        Ray(const Vec3& origin, const Vec3 direction) {
            origin_ = origin;
            direction_ = direction;
        }
        
        Vec3 getOrigin() const {return origin_;}
        Vec3 getDirection() const {return direction_;}

        void setOrigin(const Vec3& origin) {origin_ = origin;}
        void setDirection(const Vec3& direction) {direction_ = direction;}

        Vec3 pointAtParameter(double t) const {
            return origin_ + t * direction_;
        }

    private:
        Vec3 origin_, direction_;

};

#endif