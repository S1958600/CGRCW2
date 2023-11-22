#include "Light.h"

Light::Light(const Vec3& position, const Vec3& intensity)
    : position(position), intensity(intensity) {}

Vec3 Light::getPosition() const {
    return position;
}

Vec3 Light::getIntensity() const {
    return intensity;
}