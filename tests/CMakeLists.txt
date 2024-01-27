# CMakeLists.txt for your project

cmake_minimum_required(VERSION 3.10)

project(YourRaytracerProject)

# Find Google Test
find_package(GTest REQUIRED)

# Include Google Test headers
include_directories(${GTEST_INCLUDE_DIRS})

# Your raytracer source files
add_executable(YourRaytracerExecutable RayTracer.cpp)

# Link against Google Test
target_link_libraries(YourRaytracerExecutable ${GTEST_LIBRARIES} pthread)
