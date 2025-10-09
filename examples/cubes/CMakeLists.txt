add_executable(cubes main.cpp)
target_link_libraries(cubes PRIVATE
  glfw
  KumaGL
)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS cubes
  EXPORT KumaGLTargets
  RUNTIME DESTINATION ${EXAMPLE_INSTALL_DIR}/cubes
)

install(DIRECTORY resources
  DESTINATION ${EXAMPLE_INSTALL_DIR}/cubes
)
