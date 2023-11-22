#ifndef LIGHT_H
#define LIGHT_H

#include <string>
#include "Vec3.h"

class Light {
public:
    Light(const Vec3& position, const Vec3& intensity);

    Vec3 getPosition() const;
    Vec3 getIntensity() const;

private:
    Vec3 position;
    Vec3 intensity;
};

#endif // LIGHT_H