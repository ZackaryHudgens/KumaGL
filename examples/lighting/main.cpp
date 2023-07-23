#include <ctime>
#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <array>
#include <iostream>

#include <KumaGL/Mesh.hpp>
#include <KumaGL/Shader.hpp>
#include <KumaGL/Texture.hpp>
#include <KumaGL/Transform.hpp>

/******************************************************************************/
void FramebufferSizeCallback(GLFWwindow *aWindow, int aWidth, int aHeight) {
  glViewport(0, 0, aWidth, aHeight);
}

/******************************************************************************/
GLFWwindow *CreateWindow() {
  GLFWwindow *window = nullptr;

  // Initialize GLFW.
  if (!glfwInit()) {
    std::cout << "Failed to initialize GLFW!" << std::endl;
    return window;
  }

  // Set the desired OpenGL version to 3.3.
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  // Use the core profile only; this removes backwards-compatible features
  // that are no longer needed for the engine.
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Enable forward compatibility; this removes all deprecated features
  // in the desired version of OpenGL (3.3).
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);

  // Enable double buffering.
  glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

  // Create a new window.
  window = glfwCreateWindow(1280, 720, "lighting", nullptr, nullptr);
  if (window == nullptr) {
    std::cout << "Failed to create window!" << std::endl;
    return window;
  }

  // Make the context current.
  glfwMakeContextCurrent(window);

  // Set any GLFW callbacks.
  glfwSetFramebufferSizeCallback(window, &FramebufferSizeCallback);

  return window;
}

/******************************************************************************/
bool InitializeGL() {
  bool success = false;

  // Initialize GLAD.
  success = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
  if (!success) {
    std::cout << "Failed to initialize GLAD!" << std::endl;
    return success;
  }

  // Set up global OpenGL state.
  glEnable(GL_DEPTH_TEST);

  return true;
}

/******************************************************************************/
int main() {
  auto window = CreateWindow();
  if (window == nullptr) {
    return -1;
  }

  auto success = InitializeGL();
  if (!success) {
    return -1;
  }

  // Load the shader.
  KumaGL::Shader shader;
  shader.LoadFromFiles("resources/shaders/Shader.vert",
                       "resources/shaders/Shader.frag");

  // Load the textures.
  KumaGL::Texture diffuse;
  KumaGL::Texture specular;
  diffuse.LoadFromFile("resources/textures/diffuse.png");
  specular.LoadFromFile("resources/textures/specular.png");

  diffuse.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  diffuse.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  diffuse.GenerateMipmap();

  specular.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  specular.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  specular.GenerateMipmap();

  diffuse.Bind(GL_TEXTURE0);
  specular.Bind(GL_TEXTURE1);

  // Set the shader uniforms.
  shader.SetMat4("viewMatrix",
                 KumaGL::View(KumaGL::Vec3(0, 0, 1), KumaGL::Vec3(1, 0, 0),
                              KumaGL::Vec3(0, 0, 0)));
  shader.SetMat4("projectionMatrix",
                 KumaGL::Perspective(45, 1280, 720, 0.1, 100));

  shader.SetInt("diffuseTexture", 0);
  shader.SetInt("specularTexture", 1);
  shader.SetFloat("shininess", 256);

  shader.SetVec3("lightColor", KumaGL::Vec3(1, 0.9, 0.7));

  shader.SetVec3("viewPos", KumaGL::Vec3(0, 0, 0));

  // Create a cube mesh.
  KumaGL::Mesh cube;
  cube.InitCube();

  // Create a transform for the light, as well as several cubes.
  KumaGL::Transform light;
  std::array<KumaGL::Transform, 5> cubes;

  light.SetPosition(KumaGL::Vec3(0, 0, 10));
  cubes[0].SetPosition(KumaGL::Vec3(0, 0, -10));
  cubes[1].SetPosition(KumaGL::Vec3(0, 2, -10));
  cubes[2].SetPosition(KumaGL::Vec3(2, 0, -10));
  cubes[3].SetPosition(KumaGL::Vec3(0, -2, -10));
  cubes[4].SetPosition(KumaGL::Vec3(-2, 0, -10));

  // Set the light position in the shader.
  shader.SetVec3("lightPos", light.GetWorldPosition());

  // Run until instructed to close.
  while (!glfwWindowShouldClose(window)) {
    glfwSwapBuffers(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Rotate each cube object.
    for (auto &t : cubes) {
      t.Rotate(1, 1, 0);
    }

    // Copy the cube matrices to the instance buffer.
    std::vector<KumaGL::Mat4> matrices;
    for (auto &t : cubes) {
      matrices.emplace_back(t.GetMatrix());
    }

    cube.mInstanceBuffer.CopyData(GL_ARRAY_BUFFER,
                                  matrices.size() * sizeof(KumaGL::Mat4),
                                  matrices.data(), GL_DYNAMIC_DRAW);

    // Draw the cubes.
    shader.Bind();
    cube.DrawInstanced(cubes.size());

    glfwPollEvents();
  }

  shader.Delete();
  diffuse.Delete();
  specular.Delete();
  cube.Delete();

  glfwTerminate();

  return 0;
}
