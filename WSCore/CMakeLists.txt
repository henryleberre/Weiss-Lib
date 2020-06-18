# This CMakeLists.txt file build the Weiss Engine but has to be run by the main CMakeLists.txt file
# because it prepares it

# Find Weiss Engine .h && .cpp Files
file(GLOB_RECURSE WS_ENGINE_H_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/WSCore/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/WSCore/*.cpp")

# Create The Weiss Static Library Target
ADD_LIBRARY(WeissEngine STATIC "${WS_ENGINE_H_CPP_FILES}")

# Set Target Propreties
SET_TARGET_PROPERTIES(WeissEngine PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(WeissEngine PROPERTIES VERSION ${PROJECT_VERSION})

# Link
LINK_LIBRARIES(WeissEngine "${Vulkan_LIBRARIES}")

# Include Dependencies
TARGET_INCLUDE_DIRECTORIES(WeissEngine PUBLIC "${Vulkan_INCLUDE_DIRS}")