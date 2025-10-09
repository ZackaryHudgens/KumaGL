add_executable(shadows main.cpp)
target_link_libraries(shadows PUBLIC
  glfw
  KumaGL
)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS shadows
  EXPORT KumaGLTargets
  RUNTIME DESTINATION ${EXAMPLE_INSTALL_DIR}/shadows
)

install(DIRECTORY resources
  DESTINATION ${EXAMPLE_INSTALL_DIR}/shadows
)
