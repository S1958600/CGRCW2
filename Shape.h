#ifndef SHAPE_H
#define SHAPE_H

#include "Material.h"
#include "RayStructures.h"
#include "BoundingBox.h"

class Shape {
public:
    // Constructor
    Shape(const Material& material);

    // Virtual destructor
    virtual ~Shape();

    // Virtual method for ray intersection testing
    virtual bool intersect(const Ray& ray, Hit& hit, Shape* ignoreShape) const = 0;

    // Getter for material properties
    Material& getMaterial();

    virtual void calculateBoundingBox() = 0;

    virtual Vec3 getCenter() const = 0;

    // Bounding box
    BoundingBox bbox;

    bool isCloser(const double t, const Hit& hit) const {
        if (hit.t() <= 0) {
            return true;
        }

        return t < hit.t();
    }

protected:
    Material material_;
};

#endif // SHAPE_H