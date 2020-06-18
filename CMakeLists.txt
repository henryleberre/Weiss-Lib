# Setup
CMAKE_MINIMUM_REQUIRED(VERSION 3.9)

# Create Main Project
PROJECT(Weiss VERSION 0.0.1)

# Set Standard Version
SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable Unicode
ADD_DEFINITIONS(-D_UNICODE -DUNICODE)

include(GNUInstallDirs)

# Find Packages
FIND_PACKAGE(Vulkan REQUIRED FATAL_ERROR)

# Include All CMakeLists.txt Files
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/core/CMakeLists.txt")   # Weiss' Core Engine Static Library
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/sample/CMakeLists.txt") # Sample Test   Application
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/editor/CMakeLists.txt") # Weiss' Editor Application