# Create The Editor Project That Includes WeissEngine

file(GLOB_RECURSE WS_EDITOR_H_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/WSEditor/*.h"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/WSEditor/*.cpp")

ADD_EXECUTABLE(WeissEditor "${WS_EDITOR_H_CPP_FILES}")

# Set Target Propreties
SET_TARGET_PROPERTIES(WeissEditor PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(WeissEditor PROPERTIES VERSION ${PROJECT_VERSION})

# Link Dependencies
LINK_LIBRARIES(WeissEditor "${Vulkan_LIBRARIES}")

# Include
TARGET_INCLUDE_DIRECTORIES(WeissEditor PUBLIC "${Vulkan_INCLUDE_DIRS}" "${PROJECT_BINARY_DIR}/")