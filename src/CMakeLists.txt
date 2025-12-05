# Add an option for using a different OpenGL Loader than GLAD.
option(USE_INTERNAL_GLAD "Uses the version of GLAD bundled with KumaGL." ON)

# Gather all the required source files.
list(APPEND KUMAGL_SOURCES
  Framebuffer.cpp
  GLObject.cpp
  Mat4.cpp
  Mesh.cpp
  Renderbuffer.cpp
  Shader.cpp
  Texture.cpp
  VAO.cpp
  VBO.cpp
  Vec3.cpp
)

if(USE_INTERNAL_GLAD)
  list(APPEND KUMAGL_SOURCES glad/gl.c)
endif(USE_INTERNAL_GLAD)

# Create the library.
add_library(KumaGL ${KUMAGL_SOURCES})
target_include_directories(KumaGL PUBLIC
  ../include
  ../include/KumaGL
  ../external
)

# Install the library and the header files.
install(TARGETS KumaGL DESTINATION lib)
install(DIRECTORY ../include/KumaGL DESTINATION include)
