add_executable(lighting main.cpp)
target_link_libraries(lighting PUBLIC
  glfw
  KumaGL
)

file(COPY resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

install(TARGETS lighting
  EXPORT KumaGLTargets
  RUNTIME DESTINATION ${EXAMPLE_INSTALL_DIR}/lighting
)

install(DIRECTORY resources
  DESTINATION ${EXAMPLE_INSTALL_DIR}/lighting
)
