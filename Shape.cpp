#include "Shape.h"
#include "Material.h"

// Constructor definition
Shape::Shape(const Material& material) : material_(material) {}

// Destructor definition
Shape::~Shape() {}

void Shape::calculateBoundingBox() {}

// Getter for material properties
Material& Shape::getMaterial() {
    return material_;
}

// ... other Shape methods ...