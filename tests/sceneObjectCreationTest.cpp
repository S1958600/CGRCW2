#include <gtest/gtest.h>
#include "../ImportParser.h"
#include "../RayTracer.h"
#include "../Shape.h"
#include "../Vec3.h"
#include "../Light.h"

// Declare a fixture for common setup and teardown, if needed
class JSONFileExistsTestFixture : public ::testing::Test {
protected:
    // Add any common setup code here, if needed
    // For example, initializing objects or setting up a test directory
};

// Test case for jsonFileExistsTest
TEST_F(JSONFileExistsTestFixture, sceneObjectCreationTest) {
    // Arrange
    RayTracer renderer;

    //file names are form the pov of import parser

    // File name of an existing JSON file
    std::string existingFile = "imports/mirror_image.json";

    // Act & Assert
    // Test that the reader can read the file
    EXPECT_NO_THROW(parseRender(existingFile, renderer));

    // Iterate over all shapes in the scene
    for (Shape* shape : renderer.getScene().getShapes()) {
        // Get the center of the shape
        Vec3 center = shape->getCenter();

        // Print the attributes of the center
        std::cout << "Center of shape: (" << center.v[0] << ", " << center.v[1] << ", " << center.v[2] << ")" << std::endl;
    }

    // Iterate over all light sources in the scene
    for (Light* light : renderer.getScene().getLights()) {
        // Get the position of the light source
        Vec3 position = light->getPosition();

        // Print the attributes of the position
        std::cout << "Position of light: (" << position.v[0] << ", " << position.v[1] << ", " << position.v[2] << ")" << std::endl;
    }

    //Test that the raytracer can use the parser to read the file
    EXPECT_NO_THROW(renderer.parseRenderWrite(existingFile, BINARY, 1, 1));

}