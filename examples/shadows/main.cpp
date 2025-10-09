#include <KumaGL/KumaGL.hpp>

#include <GLFW/glfw3.h>

#include <array>
#include <iostream>

#include <KumaGL/Framebuffer.hpp>
#include <KumaGL/Mat4.hpp>
#include <KumaGL/Mesh.hpp>
#include <KumaGL/Renderbuffer.hpp>
#include <KumaGL/Shader.hpp>
#include <KumaGL/Texture.hpp>
#include <KumaGL/Vec3.hpp>

/******************************************************************************/
int windowWidth = 1280;
int windowHeight = 720;

bool debugMode = false;

/******************************************************************************/
struct Transform
{
  KumaGL::Vec3 mPosition;
  KumaGL::Vec3 mRotation;
  KumaGL::Vec3 mScale{1, 1, 1};

  KumaGL::Mat4 GetMatrix() const
  {
    KumaGL::Mat4 mat;
    mat.Translate(mPosition);
    mat.Rotate(mRotation);
    mat.Scale(mScale);
    return mat;
  }
};

/******************************************************************************/
struct RenderInfo
{
  KumaGL::Shader mCubeShader;
  KumaGL::Shader mDepthShader;
  KumaGL::Shader mDebugShader;

  KumaGL::Texture mCubeDiffuse;
  KumaGL::Texture mCubeSpecular;
  KumaGL::Texture mShadowMap;

  KumaGL::Mesh mCubeMesh;
  KumaGL::Mesh mQuadMesh;
  KumaGL::Mesh mDebugMesh;

  KumaGL::Framebuffer mShadowBuffer;

  void Setup()
  {
    // Configure the shaders.
    mCubeShader.LoadFromFiles("resources/shaders/Shader.vert",
                              "resources/shaders/Shader.frag");

    mCubeShader.SetInt("diffuseTexture", 0);
    mCubeShader.SetInt("specularTexture", 1);
    mCubeShader.SetInt("shadowMapTexture", 2);

    mCubeShader.SetVec3("lightColor", KumaGL::Vec3(1, 0.9, 0.7));
    mCubeShader.SetVec3("viewPos", KumaGL::Vec3(0, 0, 0));

    mCubeShader.SetFloat("shininess", 256);

    mDepthShader.LoadFromFiles("resources/shaders/DepthShader.vert",
                               "resources/shaders/DepthShader.frag");

    mDebugShader.LoadFromFiles("resources/shaders/DebugShader.vert",
                               "resources/shaders/DebugShader.frag");
    mDebugShader.SetInt("shadowMap", 2);

    // Configure the textures.
    mCubeDiffuse.LoadFromFile("resources/textures/diffuse.png");
    mCubeDiffuse.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    mCubeDiffuse.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    mCubeSpecular.LoadFromFile("resources/textures/specular.png");
    mCubeSpecular.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    mCubeSpecular.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    mShadowMap.LoadFromData(nullptr, 1280, 720, GL_DEPTH_COMPONENT, GL_FLOAT);
    mShadowMap.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    mShadowMap.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    mCubeDiffuse.Bind(GL_TEXTURE0);
    mCubeSpecular.Bind(GL_TEXTURE1);
    mShadowMap.Bind(GL_TEXTURE2);

    // Configure the meshes.
    mCubeMesh.InitCube();
    mQuadMesh.InitQuad();
    mDebugMesh.InitQuad();

    // Configure the framebuffer.
    mShadowBuffer.AttachTexture(mShadowMap, GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                                GL_TEXTURE_2D);
    mShadowBuffer.Bind();
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    mShadowBuffer.Unbind();
  }

  void Delete()
  {
    mCubeShader.Delete();
    mDepthShader.Delete();
    mDebugShader.Delete();

    mCubeDiffuse.Delete();
    mCubeSpecular.Delete();
    mShadowMap.Delete();

    mCubeMesh.Delete();
    mQuadMesh.Delete();
    mDebugMesh.Delete();

    mShadowBuffer.Delete();
  }
};

/******************************************************************************/
struct Scene
{
  Transform mPlaneTransform;
  Transform mLightTransform;
  std::array<Transform, 5> mCubeTransforms;

  void Setup()
  {
    mPlaneTransform.mPosition.y = 3;
    mPlaneTransform.mPosition.z = -20;
    mPlaneTransform.mScale.x = 10;
    mPlaneTransform.mScale.y = 10;

    mLightTransform.mPosition.z = 10;

    mCubeTransforms[0].mPosition = KumaGL::Vec3(0, 0, -10);
    mCubeTransforms[1].mPosition = KumaGL::Vec3(2, 0, -10);
    mCubeTransforms[2].mPosition = KumaGL::Vec3(-2, 0, -10);
    mCubeTransforms[3].mPosition = KumaGL::Vec3(0, 2, -10);
    mCubeTransforms[4].mPosition = KumaGL::Vec3(0, -2, -10);
  }

  void Update()
  {
    // Rotate each transform.
    for (auto &transform : mCubeTransforms)
    {
      transform.mRotation.x += 0.5;
      transform.mRotation.y += 0.5;
    }
  }

  void PreRender(RenderInfo &aInfo)
  {
    // Add each transformation matrix to a vector.
    std::vector<KumaGL::Mat4> matrices;
    for (const auto &transform : mCubeTransforms)
    {
      matrices.emplace_back(transform.GetMatrix());
    }

    // Copy the matrices into the cube instance buffer.
    aInfo.mCubeMesh.mInstanceBuffer.CopyData(
        GL_ARRAY_BUFFER, matrices.size() * sizeof(KumaGL::Mat4),
        matrices.data(), GL_DYNAMIC_DRAW);

    // Copy the quad matrix into the quad instance buffer.
    aInfo.mQuadMesh.mInstanceBuffer.CopyData(
        GL_ARRAY_BUFFER, sizeof(KumaGL::Mat4),
        mPlaneTransform.GetMatrix().GetData(), GL_DYNAMIC_DRAW);

    // Set the light position in the cube shader.
    aInfo.mCubeShader.SetVec3("lightPos", mLightTransform.mPosition);
  }

  void Render(const RenderInfo &aInfo)
  {
    aInfo.mCubeMesh.DrawInstanced(mCubeTransforms.size());
    aInfo.mQuadMesh.DrawInstanced(1);
  }
};

/******************************************************************************/
void FramebufferSizeCallback(GLFWwindow *aWindow, int aWidth, int aHeight)
{
  windowWidth = aWidth;
  windowHeight = aHeight;
}

/******************************************************************************/
void KeyPressCallback(GLFWwindow *aWindow, int aKey, int aScancode, int aAction,
                      int aMods)
{
  if (aAction == GLFW_PRESS)
  {
    if (aKey == GLFW_KEY_D)
    {
      debugMode = !debugMode;
    }
  }
}

/******************************************************************************/
GLFWwindow *CreateWindow()
{
  GLFWwindow *window = nullptr;

  // Initialize GLFW.
  if (!glfwInit())
  {
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
  window =
      glfwCreateWindow(windowWidth, windowHeight, "shadows", nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create window!" << std::endl;
    return window;
  }

  // Make the context current.
  glfwMakeContextCurrent(window);

  // Set any GLFW callbacks.
  glfwSetFramebufferSizeCallback(window, &FramebufferSizeCallback);
  glfwSetKeyCallback(window, &KeyPressCallback);

  return window;
}

/******************************************************************************/
bool InitializeGL()
{
  if (!KumaGL::InitKumaGL(glfwGetProcAddress))
  {
    std::cout << "Failed to initialize KumaGL!" << std::endl;
    return false;
  }

  // Set up global OpenGL state.
  glEnable(GL_DEPTH_TEST);

  return true;
}

/******************************************************************************/
int main()
{
  auto window = CreateWindow();
  if (window == nullptr)
  {
    return -1;
  }

  auto success = InitializeGL();
  if (!success)
  {
    return -1;
  }

  RenderInfo info;
  info.Setup();

  Scene scene;
  scene.Setup();

  // Run until instructed to close.
  while (!glfwWindowShouldClose(window))
  {
    glfwPollEvents();
    glfwSwapBuffers(window);

    // Update the scene.
    scene.Update();

    // Before rendering, copy scene data to the GPU.
    scene.PreRender(info);

    // For the first render pass, generate a depth map by rendering the scene
    // from the light's perspective and storing it in a texture.
    info.mShadowBuffer.Bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, info.mShadowMap.GetWidth(), info.mShadowMap.GetHeight());

    // Calculate the view and projection matrices for the light.
    KumaGL::Mat4 lightView;
    lightView.LookAt(scene.mLightTransform.mPosition, KumaGL::Vec3(0, 0, -10),
                     KumaGL::Vec3(0, 1, 0));

    KumaGL::Mat4 lightProj;
    lightProj.Orthographic(-10, 10, -10, 10, 0.1, 100);

    // Set the shader uniforms.
    info.mDepthShader.SetMat4("viewMatrix", lightView);
    info.mDepthShader.SetMat4("projectionMatrix", lightProj);

    // Render to the shadow framebuffer.
    info.mDepthShader.Bind();
    scene.Render(info);
    info.mShadowBuffer.Unbind();

    if (debugMode)
    {
      // Render the debug map to the screen.
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glViewport(0, 0, windowWidth, windowHeight);

      info.mDebugShader.Bind();
      info.mDebugMesh.DrawInstanced(1);
      info.mDebugShader.Unbind();
    }
    else
    {
      // For the second render pass, draw the scene as per usual from the
      // viewer's perspective.
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glViewport(0, 0, windowWidth, windowHeight);

      // Set the shader uniforms.
      KumaGL::Mat4 view;
      view.LookAt(KumaGL::Vec3(0, 0, 0), KumaGL::Vec3(0, 0, -1),
                  KumaGL::Vec3(0, 1, 0));
      info.mCubeShader.SetMat4("viewMatrix", view);

      KumaGL::Mat4 proj;
      proj.Perspective(45, 1280, 720, 0.1, 100);
      info.mCubeShader.SetMat4("projectionMatrix", proj);

      info.mCubeShader.SetMat4("lightSpaceMatrix", lightProj * lightView);
      info.mCubeShader.SetVec3("lightPos", scene.mLightTransform.mPosition);

      // Render the scene.
      info.mCubeShader.Bind();
      scene.Render(info);
      info.mCubeShader.Unbind();
    }
  }

  info.Delete();

  glfwTerminate();
  return 0;
}
