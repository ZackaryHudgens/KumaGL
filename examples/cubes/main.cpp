#include <KumaGL/KumaGL.hpp>

#include <GLFW/glfw3.h>

#include <iostream>
#include <numeric>
#include <random>
#include <vector>

#include <KumaGL/Mat4.hpp>
#include <KumaGL/Mesh.hpp>
#include <KumaGL/Shader.hpp>
#include <KumaGL/Texture.hpp>
#include <KumaGL/Vec3.hpp>

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
    KumaGL::Shader mCubeShader;
    KumaGL::Texture mCubeTexture;

    void Setup()
    {
        // Configure the meshes.
        mCubeMesh.InitCube();

        // Configure the shaders.
        mCubeShader.LoadFromFiles("resources/shaders/CubeShader.vert",
                                  "resources/shaders/CubeShader.frag");
        mCubeShader.SetInt("tex", 0);

        // Configure the textures.
        mCubeTexture.LoadFromFile("resources/texture.png");
        mCubeTexture.SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        mCubeTexture.SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        mCubeTexture.GenerateMipmap();
        mCubeTexture.Bind(GL_TEXTURE0);
    }

    void Delete()
    {
        mCubeMesh.Delete();
        mCubeShader.Delete();
        mCubeTexture.Delete();
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
    glViewport(0, 0, aWidth, aHeight);
}

/******************************************************************************/
void ErrorCallback(int aCode, const char *aDescription)
{
    std::cout << "GLFW error code: " << aCode << std::endl;
    std::cout << "Description: " << aDescription << std::endl;
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
    window = glfwCreateWindow(1280, 720, "cubes", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create window!" << std::endl;
        return window;
    }

    // Make the context current.
    glfwMakeContextCurrent(window);

    // Set any GLFW callbacks.
    glfwSetFramebufferSizeCallback(window, &FramebufferSizeCallback);
    glfwSetErrorCallback(&ErrorCallback);

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
        glfwPollEvents();
        glfwSwapBuffers(window);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.Update();
        scene.PreRender(info);
        scene.Render(info, info.mCubeShader);
    }

    info.Delete();

    glfwTerminate();

    return 0;
}
