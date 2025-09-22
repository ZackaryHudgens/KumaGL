#include "KumaGL/Renderbuffer.hpp"

#include <algorithm>

#include "KumaGL/GLObject.hpp"

namespace KumaGL
{
    /******************************************************************************/
    Renderbuffer::Renderbuffer() { Generate(); }

    /******************************************************************************/
    Renderbuffer::~Renderbuffer() { Delete(); }

    /******************************************************************************/
    Renderbuffer::Renderbuffer(Renderbuffer &&aBuffer)
        : GLObject(std::move(aBuffer)) {}

    /******************************************************************************/
    Renderbuffer &Renderbuffer::operator=(Renderbuffer &&aBuffer)
    {
        Delete();
        GLObject::operator=(std::move(aBuffer));
        return *this;
    }

    /******************************************************************************/
    void Renderbuffer::Generate()
    {
        if (!mID)
        {
            glGenRenderbuffers(1, &mID);
        }
    }

    /******************************************************************************/
    void Renderbuffer::Delete()
    {
        if (mID)
        {
            glDeleteRenderbuffers(1, &mID);
            mID = 0;
        }
    }

    /******************************************************************************/
    void Renderbuffer::Bind() const { glBindRenderbuffer(GL_RENDERBUFFER, mID); }

    /******************************************************************************/
    void Renderbuffer::Unbind() const { glBindRenderbuffer(GL_RENDERBUFFER, 0); }

    /******************************************************************************/
    void Renderbuffer::SetStorageParameters(GLenum aStorageFormat, GLsizei aWidth,
                                            GLsizei aHeight) const
    {
        Bind();
        glRenderbufferStorage(GL_RENDERBUFFER, aStorageFormat, aWidth, aHeight);
        Unbind();
    }
} // namespace KumaGL
