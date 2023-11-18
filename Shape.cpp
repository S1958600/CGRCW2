#include "Shape.h"
#include "Material.h"

// Constructor definition
Shape::Shape(const Material& material) : material_(material) {}

// Destructor definition
Shape::~Shape() {}

// Getter for material properties
const Material& Shape::getMaterial() const {
    return material_;
}
