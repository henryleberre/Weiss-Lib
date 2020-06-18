# Create A Sample Project That Includes WeissEngine

file(GLOB_RECURSE WS_SAMPLE_H_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/sample/*.h"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/sample/*.cpp")

ADD_EXECUTABLE(WeissSample "${WS_SAMPLE_H_CPP_FILES}")

# Set Target Propreties
SET_TARGET_PROPERTIES(WeissSample PROPERTIES LINKER_LANGUAGE CXX)
SET_TARGET_PROPERTIES(WeissSample PROPERTIES VERSION ${PROJECT_VERSION})

# Link Dependencies
LINK_LIBRARIES(WeissSample "${Vulkan_LIBRARIES}")

# Include
TARGET_INCLUDE_DIRECTORIES(WeissSample PUBLIC "${Vulkan_INCLUDE_DIRS}" "${PROJECT_BINARY_DIR}/")