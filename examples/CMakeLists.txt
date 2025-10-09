set(EXAMPLE_INSTALL_DIR bin/KumaGL/examples)

# Retrieve the GLFW library, which is used by each example.
include(FetchContent)

FetchContent_Declare(
  glfw3
  GIT_REPOSITORY https://github.com/glfw/glfw
  GIT_TAG 3.3.8
)

FetchContent_GetProperties(glfw3)
if(NOT glfw3_POPULATED)
  FetchContent_Populate(glfw3)
  add_subdirectory(${glfw3_SOURCE_DIR} ${glfw3_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_subdirectory(cubes)
#add_subdirectory(framebuffers)
#add_subdirectory(lighting)
#add_subdirectory(shadows)
