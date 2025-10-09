#include "KumaGL/Mesh.hpp"

#include "KumaGL/Mat4.hpp"

namespace KumaGL
{
    /******************************************************************************/
    Mesh::Mesh() { Configure(); }

    /******************************************************************************/
    void Mesh::Configure()
    {
        // Configure the vertex attributes.
        mVertexArray.ConfigureVertexAttribute(
            mVertexBuffer, 0, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
            (void *)offsetof(MeshVertex, mPosition));
        mVertexArray.ConfigureVertexAttribute(mVertexBuffer, 1, 3, GL_FLOAT, GL_FALSE,
                                              sizeof(MeshVertex),
                                              (void *)(offsetof(MeshVertex, mColor)));
        mVertexArray.ConfigureVertexAttribute(
            mVertexBuffer, 2, 3, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
            (void *)(offsetof(MeshVertex, mNormal)));
        mVertexArray.ConfigureVertexAttribute(
            mVertexBuffer, 3, 2, GL_FLOAT, GL_FALSE, sizeof(MeshVertex),
            (void *)(offsetof(MeshVertex, mTexCoords)));

        // Configure the instance attributes.
        mVertexArray.ConfigureVertexAttributeWithDivisor(
            mInstanceBuffer, 4, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4), (void *)0, 1);
        mVertexArray.ConfigureVertexAttributeWithDivisor(
            mInstanceBuffer, 5, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4),
            (void *)(4 * sizeof(float)), 1);
        mVertexArray.ConfigureVertexAttributeWithDivisor(
            mInstanceBuffer, 6, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4),
            (void *)(8 * sizeof(float)), 1);
        mVertexArray.ConfigureVertexAttributeWithDivisor(
            mInstanceBuffer, 7, 4, GL_FLOAT, GL_FALSE, sizeof(Mat4),
            (void *)(12 * sizeof(float)), 1);

        // Bind the element buffer.
        mVertexArray.SetElementBuffer(mElementBuffer);
    }

    /******************************************************************************/
    void Mesh::Draw(GLenum aMode) const
    {
        mVertexArray.Bind();
        glDrawElements(aMode, mIndices.size(), GL_UNSIGNED_INT, 0);
    }

    /******************************************************************************/
    void Mesh::DrawInstanced(int aNumInstances, GLenum aMode) const
    {
        mVertexArray.Bind();
        glDrawElementsInstanced(aMode, mIndices.size(), GL_UNSIGNED_INT, 0,
                                aNumInstances);
    }

    /******************************************************************************/
    void Mesh::UpdateVertices(GLenum aUsage)
    {
        mVertexBuffer.CopyData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(MeshVertex),
                               mVertices.data(), aUsage);
    }

    /******************************************************************************/
    void Mesh::UpdateIndices(GLenum aUsage)
    {
        mElementBuffer.CopyData(GL_ELEMENT_ARRAY_BUFFER,
                                mIndices.size() * sizeof(unsigned int),
                                mIndices.data(), aUsage);
    }

    /******************************************************************************/
    void Mesh::InitQuad()
    {
        mVertices.clear();
        mIndices.clear();

        MeshVertex vertex;

        vertex.mPosition = Vec3(-0.5, -0.5, 0);
        vertex.mNormal = Vec3(0, 0, 1);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, -0.5, 0);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, 0.5, 0);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(-0.5, 0.5, 0);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);

        mIndices.emplace_back(3);
        mIndices.emplace_back(0);
        mIndices.emplace_back(1);
        mIndices.emplace_back(3);
        mIndices.emplace_back(1);
        mIndices.emplace_back(2);

        UpdateVertices();
        UpdateIndices();
    }

    /******************************************************************************/
    void Mesh::InitCube()
    {
        mVertices.clear();
        mIndices.clear();

        MeshVertex vertex;

        // Front face
        vertex.mPosition = Vec3(-0.5, -0.5, 0.5);
        vertex.mNormal = Vec3(0, 0, 1);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, -0.5, 0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, 0.5, 0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(-0.5, 0.5, 0.5);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);

        mIndices.emplace_back(0);
        mIndices.emplace_back(1);
        mIndices.emplace_back(3);
        mIndices.emplace_back(1);
        mIndices.emplace_back(2);
        mIndices.emplace_back(3);

        // Top face
        vertex.mPosition = Vec3(-0.5, 0.5, 0.5);
        vertex.mNormal = Vec3(0, 1, 0);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, 0.5, 0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, 0.5, -0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(-0.5, 0.5, -0.5);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);

        mIndices.emplace_back(4);
        mIndices.emplace_back(5);
        mIndices.emplace_back(7);
        mIndices.emplace_back(5);
        mIndices.emplace_back(6);
        mIndices.emplace_back(7);

        // Back face
        vertex.mPosition = Vec3(-0.5, -0.5, -0.5);
        vertex.mNormal = Vec3(0, 0, -1);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, -0.5, -0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, 0.5, -0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(-0.5, 0.5, -0.5);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);

        mIndices.emplace_back(8);
        mIndices.emplace_back(9);
        mIndices.emplace_back(11);
        mIndices.emplace_back(9);
        mIndices.emplace_back(10);
        mIndices.emplace_back(11);

        // Left face
        vertex.mPosition = Vec3(-0.5, -0.5, -0.5);
        vertex.mNormal = Vec3(-1, 0, 0);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(-0.5, -0.5, 0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(-0.5, 0.5, 0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(-0.5, 0.5, -0.5);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);

        mIndices.emplace_back(12);
        mIndices.emplace_back(13);
        mIndices.emplace_back(15);
        mIndices.emplace_back(13);
        mIndices.emplace_back(14);
        mIndices.emplace_back(15);

        // Bottom face
        vertex.mPosition = Vec3(-0.5, -0.5, -0.5);
        vertex.mNormal = Vec3(0, -1, 0);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, -0.5, -0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, -0.5, 0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(-0.5, -0.5, 0.5);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);

        mIndices.emplace_back(16);
        mIndices.emplace_back(17);
        mIndices.emplace_back(19);
        mIndices.emplace_back(17);
        mIndices.emplace_back(18);
        mIndices.emplace_back(19);

        // Right face
        vertex.mPosition = Vec3(0.5, -0.5, 0.5);
        vertex.mNormal = Vec3(1, 0, 0);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, -0.5, -0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 0;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, 0.5, -0.5);
        vertex.mTexCoords[0] = 1;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);
        vertex.mPosition = Vec3(0.5, 0.5, 0.5);
        vertex.mTexCoords[0] = 0;
        vertex.mTexCoords[1] = 1;
        mVertices.emplace_back(vertex);

        mIndices.emplace_back(20);
        mIndices.emplace_back(21);
        mIndices.emplace_back(23);
        mIndices.emplace_back(21);
        mIndices.emplace_back(22);
        mIndices.emplace_back(23);

        UpdateVertices();
        UpdateIndices();
    }
} // namespace KumaGL
