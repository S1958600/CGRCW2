#include <gtest/gtest.h>
#include "../ImportParser.h"
#include "../RayTracer.h"

// Declare a fixture for common setup and teardown, if needed
class JSONFileExistsTestFixture : public ::testing::Test {
protected:
    // Add any common setup code here, if needed
    // For example, initializing objects or setting up a test directory
};

// Test case for jsonFileExistsTest
TEST_F(JSONFileExistsTestFixture, sceneComplexityTest) {
    // Arrange
    RayTracer renderer;

    //file names are form the pov of import parser

    // File name of an existing JSON file
    std::string existingFile = "imports/complex.json";

    // Act & Assert
    // Test that the reader can read the file
    EXPECT_NO_THROW(parseRender(existingFile, renderer));

    //Test that the raytracer can use the parser to read the file
    EXPECT_NO_THROW(renderer.parseRenderWrite(existingFile, BINARY, 1, 1));

}

// Add more test cases or fixtures as needed
