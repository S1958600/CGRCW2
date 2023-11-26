#ifndef RAYH
#define RAYH

#include "Vec3.h"
#include "Material.h"

class Shape; // Forward declaration

class Hit {
public:
    // Constructors
    Hit() : hitOccurred_(false), t_(-1.0f) {}
    Hit(bool hitOccurred, double t, const Vec3& location, const Vec3& normal, const Material* material)
        : hitOccurred_(hitOccurred), t_(t), location_(location), normal_(normal), material_(material) {}

    // Accessor methods
    bool hitOccurred() const { return hitOccurred_; }
    double t() const { return t_; }
    Vec3 location() const { return location_; }
    Vec3 normal() const { return normal_; }
    const Material* material() const { return material_; }
    Shape* shape() { return shape_; }

    // Mutator methods
    void setT(double t) { t_ = t; }
    void setShape(Shape* shape) { shape_ = shape; }


private:
    bool hitOccurred_; // Indicates if a hit occurred
    double t_; // Parameter along the ray where the intersection occurs
    Vec3 location_; // Coordinates of the intersection point
    Vec3 normal_; // Normal at the intersection point
    const Material* material_; // Material of the intersected object
    Shape* shape_;

};


class Ray {
public:
    Ray() : isInside_(false) {}
    Ray(const Vec3& origin, const Vec3 direction, bool isInside = false) 
        : origin_(origin), direction_(direction), isInside_(isInside) {}

    Vec3 getOrigin() const {return origin_;}
    Vec3 getDirection() const {return direction_;}
    bool isInside() const {return isInside_;}

    void setOrigin(const Vec3& origin) {origin_ = origin;}
    void setDirection(const Vec3& direction) {direction_ = direction;}
    void setInside(bool inside) {isInside_ = inside;}

    Vec3 pointAtParameter(double t) const {
        return origin_ + t * direction_;
    }

private:
    Vec3 origin_, direction_;
    bool isInside_;
};

#endif