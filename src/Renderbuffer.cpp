#include "KumaGL/Renderbuffer.hpp"

#include <algorithm>

#include "KumaGL/GLObject.hpp"

namespace KumaGL
{
    /******************************************************************************/
    Renderbuffer::Renderbuffer() : mID(0),
                                   mValid(false)
    {
        glGenRenderbuffers(1, &mID);
        mValid = true;
    }

    /******************************************************************************/
    Renderbuffer::~Renderbuffer()
    {
        glDeleteRenderbuffers(1, &mID);
        mValid = false;
    }

    /******************************************************************************/
    Renderbuffer::Renderbuffer(Renderbuffer &&aBuffer)
    {
        mID = aBuffer.mID;
        mValid = true;
        aBuffer.mValid = false;
    }

    /******************************************************************************/
    Renderbuffer &Renderbuffer::operator=(Renderbuffer &&aBuffer)
    {
        mID = aBuffer.mID;
        mValid = true;
        aBuffer.mValid = false;
        return *this;
    }

    /******************************************************************************/
    void Renderbuffer::Bind() const
    {
        if (mValid)
        {
            glBindRenderbuffer(GL_RENDERBUFFER, mID);
        }
    }

    /******************************************************************************/
    void Renderbuffer::SetStorageParameters(GLenum aStorageFormat,
                                            GLsizei aWidth,
                                            GLsizei aHeight) const
    {
        Bind();
        glRenderbufferStorage(GL_RENDERBUFFER, aStorageFormat, aWidth, aHeight);
    }
} // namespace KumaGL
