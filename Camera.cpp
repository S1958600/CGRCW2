#include "Camera.h"

#include <cmath>

Camera::Camera(const int width, const int height,const Vec3& position, const Vec3& lookAt, const Vec3& upVector, float fov, float exposure)
    : width_(width), height_(height),position_(position), lookAt_(lookAt), upVector_(upVector), fov_(fov) {}

Ray Camera::generateRay(int x, int y, int imageWidth, int imageHeight) const {
    float aspectRatio = static_cast<float>(imageWidth) / static_cast<float>(imageHeight);

    // Calculate screen coordinates
    float screenX = (2.0f * static_cast<float>(x) / static_cast<float>(imageWidth)) - 1.0f;
    float screenY = 1.0f - (2.0f * static_cast<float>(y) / static_cast<float>(imageHeight));

    // Calculate camera space coordinates
    float cameraX = screenX * aspectRatio * std::tan(fov_ / 2.0f);
    float cameraY = screenY * std::tan(fov_ / 2.0f);

    // Compute direction vector using the camera parameters
    Vec3 direction = (lookAt_ - position_).make_normalised(); // Normalize lookAt - position
    Vec3 right = cross(direction, upVector_).make_normalised(); // Compute right vector
    Vec3 up = cross(right, direction).make_normalised(); // Compute up vector

    direction = (cameraX * right) + (cameraY * up) + direction;

    // Return the ray from camera position through the calculated direction
    return Ray(position_, direction);
}


Ray** Camera::generateAllRays(int imageWidth, int imageHeight) const {
    
    // Allocate memory for the 2D array
    Ray** rays = new Ray*[imageHeight];
    for (int i = 0; i < imageHeight; ++i) {
        rays[i] = new Ray[imageWidth];
    }

    // Generate a ray for each pixel
    for (int y = 0; y < imageHeight; ++y) {
        for (int x = 0; x < imageWidth; ++x) {
            rays[y][x] = generateRay(x, y, imageWidth, imageHeight);
        }
    }
    printf("Generated all rays %d x %d array\n", imageHeight, imageWidth);

    return rays;
}

Ray* Camera::generateAllRays1D(int imageWidth, int imageHeight) const {
    // Allocate memory for the 1D array
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