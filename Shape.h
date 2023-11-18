#ifndef SHAPE_H
#define SHAPE_H

#include "Material.h"
#include "RayStructures.h"       


class Shape {
public:
    // Constructor
    Shape(const Material& material);

    // Virtual destructor
    virtual ~Shape();

    // Virtual method for ray intersection testing
    virtual bool intersect(const Ray& ray, Hit& hit) const = 0;

    // Getter for material properties
    const Material& getMaterial() const;

protected:
    Material material_;
};

#endif // SHAPE_H
