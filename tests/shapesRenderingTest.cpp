#include <gtest/gtest.h>
#include "../RayTracer.h"
#include <chrono>

// Declare a fixture for common setup and teardown, if needed
class JSONFileExistsTestFixture : public ::testing::Test {
protected:
    // Add any common setup code here, if needed
    // For example, initializing objects or setting up a test directory
};

// Test case for jsonFileExistsTest
TEST_F(JSONFileExistsTestFixture, shapesRenderingTest) {
    // Arrange
    RayTracer renderer;

    // File name of an existing JSON file
    std::string existingFile = "imports/complex.json";

    // Start the timer
    auto start = std::chrono::high_resolution_clock::now();

    // Call parseRenderWrite
    renderer.parseRenderWrite(existingFile, BINARY, 1, 1);

    // Stop the timer
    auto end = std::chrono::high_resolution_clock::now();

    // Calculate the duration
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start).count();

    // Print the duration
    std::cout << "Time taken by parseRenderWrite: " << duration << " seconds" << std::endl;
}