# Setup
CMAKE_MINIMUM_REQUIRED(VERSION 3.9)

# Create Main Project
PROJECT(Weiss VERSION 0.0.1)

# Set Standard Version
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable Unicode
ADD_DEFINITIONS(-D_UNICODE -DUNICODE)

include(GNUInstallDirs)

# Find Packages
FIND_PACKAGE(Vulkan REQUIRED)
FIND_PACKAGE(X11 REQUIRED)

# Include All CMakeLists.txt Files
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/WSCore/CMakeLists.txt")   # Weiss' Core Engine Static Library
INCLUDE("${CMAKE_CURRENT_SOURCE_DIR}/WSSample/CMakeLists.txt") # Sample Test   Application
