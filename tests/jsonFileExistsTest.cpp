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
TEST_F(JSONFileExistsTestFixture, jsonFileExistsTest) {
    // Arrange
    RayTracer renderer;

    //file names are form the pov of import parser

    // File name of an existing JSON file
    std::string existingFile = "imports/scene.json";

    // File name of a non-existing JSON file
    std::string nonExistingFile = "imports/non_existing_scene.json";

    // Act & Assert
    // Test that the method correctly detects the existence of an existing JSON file
    EXPECT_NO_THROW(parseRender(existingFile, renderer));

    // Test that the method throws an exception for a non-existing JSON file
    EXPECT_THROW(parseRender(nonExistingFile, renderer), std::runtime_error);
}

// Add more test cases or fixtures as needed
