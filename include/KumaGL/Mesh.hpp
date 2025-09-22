#ifndef MESH_HPP
#define MESH_HPP

#include <vector>

#include "KumaGL.hpp"

#include "VAO.hpp"
#include "VBO.hpp"
#include "Vec3.hpp"

namespace KumaGL
{
    struct MeshVertex
    {
        Vec3 mPosition;
        Vec3 mColor;
        Vec3 mNormal;
        float mTexCoords[2]{0.0, 0.0};
    };

    class Mesh
    {
    public:
        Mesh();
        ~Mesh() = default;
        Mesh(Mesh &&aMesh) = default;
        Mesh &operator=(Mesh &&aMesh) = default;
        Mesh(const Mesh &aMesh) = delete;
        Mesh &operator=(const Mesh &aMesh) = delete;

        void Generate();
        void Configure();
        void Delete();

        void Draw(GLenum aMode = GL_TRIANGLES) const;
        void DrawInstanced(int aNumInstances, GLenum aMode = GL_TRIANGLES) const;

        void UpdateVertices(GLenum aUsage = GL_STATIC_DRAW);
        void UpdateIndices(GLenum aUsage = GL_STATIC_DRAW);

        void InitQuad();
        void InitCube();

        std::vector<MeshVertex> mVertices;
        std::vector<unsigned int> mIndices;

        VAO mVertexArray;

        VBO mVertexBuffer;
        VBO mInstanceBuffer;
        VBO mCustomBuffer;
        VBO mElementBuffer;
    };
} // namespace KumaGL

#endif // !MESH_HPP
