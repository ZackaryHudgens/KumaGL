#include "KumaGL/VBO.hpp"
#include "KumaGL/GLObject.hpp"

#include <algorithm>

namespace KumaGL
{
    /******************************************************************************/
    VBO::VBO() : mValid(false),
                 mID(0)
    {
        glGenBuffers(1, &mID);
        mValid = true;
    }

    /******************************************************************************/
    VBO::~VBO()
    {
        if (mValid)
        {
            glDeleteBuffers(1, &mID);
            mValid = false;
        }
    }

    /******************************************************************************/
    VBO::VBO(VBO &&aBuffer)
    {
        mID = aBuffer.mID;
        mValid = true;
        aBuffer.mValid = false;
    }

    /******************************************************************************/
    VBO &VBO::operator=(VBO &&aBuffer)
    {
        mID = aBuffer.mID;
        mValid = true;
        aBuffer.mValid = false;
        return *this;
    }

    /******************************************************************************/
    void VBO::Bind(GLenum aTarget) const { glBindBuffer(aTarget, mID); }

    /******************************************************************************/
    void VBO::CopyData(GLenum aTarget, GLsizeiptr aSize, const void *aData,
                       GLenum aUsage) const
    {
        Bind(aTarget);
        glBufferData(aTarget, aSize, aData, aUsage);
    }

    /******************************************************************************/
    void VBO::CopySubData(GLenum aTarget, GLintptr aOffset, GLsizeiptr aSize,
                          const void *aData) const
    {
        Bind(aTarget);
        glBufferSubData(aTarget, aOffset, aSize, aData);
    }
} // namespace KumaGL
