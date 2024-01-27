#include <gtest/gtest.h>
#include "../ImportParser.h"
#include "../RayTracer.h"

// Declare a fixture for common setup and teardown, if needed
class JSONFileExistsTestFixture : public ::testing::Test {
protected:
    // Add any common setup code here, if needed
    // For example, initializing objects or setting up a test directory
};


// Test case for jsonFileContentValidationTest
TEST_F(JSONFileExistsTestFixture, jsonFileContentValidationTest) {
    // Arrange
    RayTracer renderer;

    // File name of a JSON file
    std::string jsonFile = "imports/scene.json";
    int numObjectsInFile = 8;

    // Act
    // Parse the JSON file and render the scene
    parseRender(jsonFile, renderer);

    // Get the number of objects in the renderer
    int numObjectsInRenderer = renderer.getScene().getLights().size() + renderer.getScene().getShapes().size(); 
    // Assert
    // Test that the number of objects in the renderer matches the number of objects in the JSON file
    EXPECT_EQ(numObjectsInRenderer, numObjectsInFile);
}

// Additional test case for another file
TEST_F(JSONFileExistsTestFixture, jsonEmptyFileValidationTest) {
    // Arrange
    RayTracer renderer;

    // File name of another JSON file
    std::string anotherJsonFile = "imports/empty.json";
    // Act
    // Parse the JSON file and render the scene
    parseRender(anotherJsonFile, renderer);

    // Get the number of objects in the renderer
    int numObjectsInRenderer = renderer.getScene().getLights().size() + renderer.getScene().getShapes().size(); 

    // Assert
    // Test that the number of objects in the renderer matches the number of objects in the JSON file
    EXPECT_EQ(numObjectsInRenderer, 0);
}