// BoundingBox.cpp
#include "BoundingBox.h"

BoundingBox::BoundingBox() : min(Vec3()), max(Vec3()) {}

BoundingBox::BoundingBox(const Vec3& min, const Vec3& max) : min(min), max(max) {}