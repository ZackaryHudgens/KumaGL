#include <KumaGL/KumaGL.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <random>

#include "KumaGL/Mat4.hpp"
#include <KumaGL/Framebuffer.hpp>
#include <KumaGL/Mesh.hpp>
#include <KumaGL/Renderbuffer.hpp>
#include <KumaGL/Shader.hpp>
#include <KumaGL/Texture.hpp>

/******************************************************************************/
int windowWidth = 1280;
int windowHeight = 720;

/******************************************************************************/
struct Transform
{
    KumaGL::Vec3 mPosition;
    KumaGL::Vec3 mRotation;

    KumaGL::Mat4 GetMatrix() const
    {
        KumaGL::Mat4 mat;
        mat.Translate(mPosition);
        mat.Rotate(mRotation);
        return mat;
    }
};

/******************************************************************************/
Transform CreateRandomTransform(std::random_device &aDevice)
{
    Transform t;

    std::mt19937 generator(aDevice());
    std::uniform_real_distribution<> dist(-25, 25);

    t.mPosition.x = dist(generator);
    t.mPosition.y = dist(generator);
    t.mPosition.z = dist(generator) - 50;

    return t;
}

/******************************************************************************/
struct RenderInfo
{
    KumaGL::Mesh mCubeMesh;
    KumaGL::Mesh mScreenMesh;

    KumaGL::Shader mCubeShader;
    KumaGL::Shader mScreenShader;

    KumaGL::Texture mCubeTexture;
    KumaGL::Texture mScreenTexture;

    KumaGL::Framebuffer mFramebuffer;
    KumaGL::Renderbuffer mRenderbuffer;

    void Setup()
    {
        // Configure the meshes.
        mCubeMesh.InitCube();
        mScreenMesh.InitQuad();

        // Configure the shaders.
        mCubeShader.LoadFromFiles("resources/shaders/CubeShader.vert",
                                  "resources/shaders/CubeShader.frag");
        mCubeShader.SetInt("tex", 0);

        mScreenShader.LoadFromFiles("resources/shaders/ScreenShader.vert",
                                    "resources/shaders/ScreenShader.frag");
        mScreenShader.SetInt("tex", 1);

        // Configure the textures.
        mCubeTexture.LoadFromFile("resources/texture.png");
        mCubeTexture.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        mCubeTexture.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        mCubeTexture.GenerateMipmap();

        mScreenTexture.LoadFromData(nullptr, 1280, 720);
        mScreenTexture.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        mScreenTexture.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        mCubeTexture.Bind(GL_TEXTURE0);
        mScreenTexture.Bind(GL_TEXTURE1);

        // Configure the renderbuffer.
        mRenderbuffer.SetStorageParameters(GL_DEPTH24_STENCIL8, 1280, 720);

        // Configure the framebuffer.
        mFramebuffer.AttachTexture(mScreenTexture);
        mFramebuffer.AttachRenderbuffer(mRenderbuffer);
    }

    void Delete()
    {
        mCubeMesh.Delete();
        mScreenMesh.Delete();

        mCubeShader.Delete();
        mScreenShader.Delete();

        mCubeTexture.Delete();
        mScreenTexture.Delete();

        mFramebuffer.Delete();
        mRenderbuffer.Delete();
    }
};

/******************************************************************************/
struct Scene
{
    std::vector<Transform> mCubeTransforms;

    void Setup()
    {
        mCubeTransforms.clear();

        // Generate a number of random transforms.
        std::random_device rd;
        for (size_t i = 0; i < 10000; ++i)
        {
            mCubeTransforms.emplace_back(CreateRandomTransform(rd));
        }
    }

    void Update()
    {
        // Rotate each transform.
        for (auto &transform : mCubeTransforms)
        {
            transform.mRotation.x += 1;
            transform.mRotation.y += 1;
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
    }

    void Render(RenderInfo &aInfo, KumaGL::Shader &aShader)
    {
        aShader.Bind();
        aInfo.mCubeMesh.DrawInstanced(mCubeTransforms.size());
        aShader.Unbind();
    }
};

/******************************************************************************/
void FramebufferSizeCallback(GLFWwindow *aWindow, int aWidth, int aHeight)
{
    windowWidth = aWidth;
    windowHeight = aHeight;
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
    window = glfwCreateWindow(1280, 720, "framebuffers", nullptr, nullptr);
    if (window == nullptr)
    {
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

    // Set the shader uniforms.
    KumaGL::Mat4 view;
    view.LookAt(KumaGL::Vec3(0, 0, 0), KumaGL::Vec3(0, 0, -1),
                KumaGL::Vec3(0, 1, 0));
    info.mCubeShader.SetMat4("viewMatrix", view);

    KumaGL::Mat4 proj;
    proj.Perspective(45, 1280, 720, 0.1, 100);
    info.mCubeShader.SetMat4("projectionMatrix", proj);

    // Run until instructed to close.
    while (!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();

        scene.Update();
        scene.PreRender(info);

        // For the first render pass, bind the framebuffer and render the scene to a
        // texture.
        info.mFramebuffer.Bind();

        glViewport(0, 0, info.mScreenTexture.GetWidth(),
                   info.mScreenTexture.GetHeight());
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.Render(info, info.mCubeShader);

        info.mFramebuffer.Unbind();

        // For the second render pass, draw the texture on a quad that spans the
        // window.
        glViewport(0, 0, windowWidth, windowHeight);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Render the screen texture.
        info.mScreenShader.Bind();
        info.mScreenMesh.DrawInstanced(1);
        info.mScreenShader.Unbind();
    }

    info.Delete();

    glfwTerminate();
    return 0;
}
