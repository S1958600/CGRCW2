#ifndef CAMERA_H
#define CAMERA_H

#include "RayStructures.h"

class Camera {
public:
    // Constructor
    Camera(){}; // Default constructor
    Camera(const int width, const int height,const Vec3& position, const Vec3& lookAt, const Vec3& upVector, float fov, float exposure);

    // Method to generate a ray from the camera viewpoint through a pixel
    Ray generateRay(int x, int y, int imageWidth, int imageHeight) const;

    // Method to generate all rays required to create a full image
    Ray** generateAllRays(int imageWidth, int imageHeight) const;

    Ray* generateAllRays1D(int imageWidth, int imageHeight) const;

    // Getters
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    Vec3 getPosition() const { return position_; }
    Vec3 getLookAt() const { return lookAt_; }
    Vec3 getUpVector() const { return upVector_; }
    float getFov() const { return fov_; }
    float getExposure() const { return exposure_; }

private:
    int width_;
    int height_;
    Vec3 position_;
    Vec3 lookAt_;
    Vec3 upVector_;
    float fov_;
    float exposure_;
};

#endif // CAMERA_H
