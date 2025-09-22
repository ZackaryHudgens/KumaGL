#include "KumaGL/VAO.hpp"
#include "KumaGL/GLObject.hpp"

#include <algorithm>

namespace KumaGL
{
    /******************************************************************************/
    VAO::VAO() { Generate(); }

    /******************************************************************************/
    VAO::~VAO() { Delete(); }

    /******************************************************************************/
    VAO::VAO(VAO &&aObject) : GLObject(std::move(aObject)) {}

    /******************************************************************************/
    VAO &VAO::operator=(VAO &&aObject)
    {
        Delete();
        GLObject::operator=(std::move(aObject));
        return *this;
    }

    /******************************************************************************/
    void VAO::Generate()
    {
        if (!mID)
        {
            glGenVertexArrays(1, &mID);
        }
    }

    /******************************************************************************/
    void VAO::Delete()
    {
        if (mID)
        {
            glDeleteVertexArrays(1, &mID);
            mID = 0;
        }
    }

    /******************************************************************************/
    void VAO::Bind() const { glBindVertexArray(mID); }

    /******************************************************************************/
    void VAO::Unbind() const { glBindVertexArray(0); }

    /******************************************************************************/
    void VAO::ConfigureVertexAttribute(VBO &aBuffer, GLuint aIndex, GLint aSize,
                                       GLenum aType, GLboolean aNormalized,
                                       GLsizei aStride, const void *aOffset) const
    {
        Bind();
        aBuffer.Bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(aIndex);
        glVertexAttribPointer(aIndex, aSize, aType, aNormalized, aStride, aOffset);
        aBuffer.Unbind(GL_ARRAY_BUFFER);
        Unbind();
    }

    /******************************************************************************/
    void VAO::ConfigureVertexAttributeWithDivisor(VBO &aBuffer, GLuint aIndex,
                                                  GLint aSize, GLenum aType,
                                                  GLboolean aNormalized,
                                                  GLsizei aStride,
                                                  const void *aOffset,
                                                  GLuint aDivisor) const
    {
        Bind();
        aBuffer.Bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(aIndex);
        glVertexAttribPointer(aIndex, aSize, aType, aNormalized, aStride, aOffset);
        glVertexAttribDivisor(aIndex, aDivisor);
        aBuffer.Unbind(GL_ARRAY_BUFFER);
        Unbind();
    }

    /******************************************************************************/
    void VAO::SetElementBuffer(VBO &aBuffer) const
    {
        Bind();
        aBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
        Unbind();
        aBuffer.Unbind(GL_ELEMENT_ARRAY_BUFFER);
    }
} // namespace KumaGL
