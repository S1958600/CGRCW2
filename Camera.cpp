#include "Camera.h"

#include <cmath>
#include <cassert>

Camera::Camera(const int width, const int height,const Vec3& position, const Vec3& lookAt, const Vec3& upVector, double fov, double exposure)
    : width_(width), height_(height),position_(position), lookAt_(lookAt), upVector_(upVector), fov_(fov) {samples_ = 1;}

Camera::Camera(const int width, const int height,const Vec3& position, const Vec3& lookAt, const Vec3& upVector, double fov, double exposure, int samples)
    : width_(width), height_(height),position_(position), lookAt_(lookAt), upVector_(upVector), fov_(fov), samples_(samples){}


Ray Camera::generateRay(int x, int y, int imageWidth, int imageHeight) const {
    double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);

    // Generate a random number between 0 and 1
    double randomX = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);
    double randomY = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);

    // Calculate screen coordinates
    double screenX = 1.0f - (2.0f * (static_cast<double>(x) + randomX) / static_cast<double>(imageWidth));
    double screenY = 1.0f - (2.0f * (static_cast<double>(y) + randomY) / static_cast<double>(imageHeight));

    // Convert FOV to radians
    double fovRadians = fov_ * M_PI / 180.0;

    // Calculate camera space coordinates
    double cameraX = screenX * aspectRatio * std::tan(fovRadians / 2.0f);
    double cameraY = screenY * std::tan(fovRadians / 2.0f);

    // Compute direction vector using the camera parameters
    Vec3 direction = (lookAt_ - position_).make_normalised(); // Normalize lookAt - position
    Vec3 right = cross(direction, upVector_).make_normalised(); // Compute right vector
    Vec3 up = cross(right, direction).make_normalised(); // Compute up vector

    direction = ((cameraX * right) + (cameraY * up) + direction).make_normalised();

    // Return the ray from camera position through the calculated direction
    return Ray(position_, direction);
}


Ray* Camera::generateAllRays() const {
    // Allocate memory for the array
    Ray* rays = new Ray[height_ * width_ * samples_];

    // Generate multiple rays for each pixel
    for (int y = height_ - 1; y >= 0; --y) {
        for (int x = width_ - 1; x >= 0; --x) {
            for (int s = 0; s < samples_; ++s) {
                int index = ((height_ - y - 1) * width_ + x) * samples_ + s;
                rays[index] = generateRay(x, y, width_, height_);
                //printf("Ray %d, %f, %f, %f generated\n", index, rays[index].getDirection()[0], rays[index].getDirection()[1], rays[index].getDirection()[2]);
            }
        }
    }

    return rays;
}