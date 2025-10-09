#include "KumaGL/VAO.hpp"
#include "KumaGL/GLObject.hpp"

#include <algorithm>

namespace KumaGL
{
    /******************************************************************************/
    VAO::VAO() : mValid(false),
                 mID(0)
    {
        glGenVertexArrays(1, &mID);
        mValid = true;
    }

    /******************************************************************************/
    VAO::~VAO()
    {
        if (mValid)
        {
            glDeleteVertexArrays(1, &mID);
            mValid = false;
        }
    }

    /******************************************************************************/
    VAO::VAO(VAO &&aObject)
    {
        mID = aObject.mID;
        mValid = true;
        aObject.mValid = false;
    }

    /******************************************************************************/
    VAO &VAO::operator=(VAO &&aObject)
    {
        mID = aObject.mID;
        mValid = true;
        aObject.mValid = false;
        return *this;
    }

    /******************************************************************************/
    void VAO::Bind() const { glBindVertexArray(mID); }

    /******************************************************************************/
    void VAO::ConfigureVertexAttribute(VBO &aBuffer, GLuint aIndex, GLint aSize,
                                       GLenum aType, GLboolean aNormalized,
                                       GLsizei aStride, const void *aOffset) const
    {
        Bind();
        aBuffer.Bind(GL_ARRAY_BUFFER);
        glEnableVertexAttribArray(aIndex);
        glVertexAttribPointer(aIndex, aSize, aType, aNormalized, aStride, aOffset);
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
    }

    /******************************************************************************/
    void VAO::SetElementBuffer(VBO &aBuffer) const
    {
        Bind();
        aBuffer.Bind(GL_ELEMENT_ARRAY_BUFFER);
    }
} // namespace KumaGL
