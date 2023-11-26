// BoundingBox.h
#pragma once

#include "Vec3.h"

struct BoundingBox {
    Vec3 min;
    Vec3 max;

    BoundingBox();
    BoundingBox(const Vec3& min, const Vec3& max);
};