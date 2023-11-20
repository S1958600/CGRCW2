#include "Camera.h"

#include <cmath>

Camera::Camera(const int width, const int height,const Vec3& position, const Vec3& lookAt, const Vec3& upVector, double fov, double exposure)
    : width_(width), height_(height),position_(position), lookAt_(lookAt), upVector_(upVector), fov_(fov) {}

Ray Camera::generateRay(int x, int y, int imageWidth, int imageHeight) const {
    double aspectRatio = static_cast<double>(imageWidth) / static_cast<double>(imageHeight);

    // Calculate screen coordinates
    double screenX = (2.0f * static_cast<double>(x) / static_cast<double>(imageWidth)) - 1.0f;
    double screenY = 1.0f - (2.0f * static_cast<double>(y) / static_cast<double>(imageHeight));

    // Calculate camera space coordinates
    double cameraX = screenX * aspectRatio * std::tan(fov_ / 2.0f);
    double cameraY = screenY * std::tan(fov_ / 2.0f);

    // Compute direction vector using the camera parameters
    Vec3 direction = (lookAt_ - position_).make_normalised(); // Normalize lookAt - position
    Vec3 right = cross(direction, upVector_).make_normalised(); // Compute right vector
    Vec3 up = cross(right, direction).make_normalised(); // Compute up vector

    direction = (cameraX * right) + (cameraY * up) + direction;

    // Return the ray from camera position through the calculated direction
    return Ray(position_, direction);
}



Ray* Camera::generateAllRays(int imageWidth, int imageHeight) const {
    // Allocate memory for the array
    Ray* rays = new Ray[imageHeight * imageWidth];

    // Generate a ray for each pixel
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            int index = y * imageWidth + x;
            rays[index] = generateRay(x, y, imageWidth, imageHeight);
        }
    }

    printf("Generated all rays %d x %d array\n", imageHeight, imageWidth);

    return rays;
}