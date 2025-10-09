#include "KumaGL/Framebuffer.hpp"

#include "KumaGL/GLObject.hpp"
#include "KumaGL/Renderbuffer.hpp"

namespace KumaGL
{
    /******************************************************************************/
    Framebuffer::Framebuffer() : mID(0),
                                 mValid(false)
    {
        glGenFramebuffers(1, &mID);
        mValid = true;
    }

    /******************************************************************************/
    Framebuffer::~Framebuffer()
    {
        if (mValid)
        {
            glDeleteFramebuffers(1, &mID);
            mValid = false;
        }
    }

    /******************************************************************************/
    Framebuffer::Framebuffer(Framebuffer &&aBuffer)
    {
        mID = aBuffer.mID;
        mValid = true;
        aBuffer.mValid = false;
    }

    /******************************************************************************/
    Framebuffer &Framebuffer::operator=(Framebuffer &&aBuffer)
    {
        mID = aBuffer.mID;
        mValid = true;
        aBuffer.mValid = false;
        return *this;
    }

    /******************************************************************************/
    void Framebuffer::Bind(GLenum aTarget) const
    {
        if (mValid)
        {
            glBindFramebuffer(aTarget, mID);
        }
    }

    /******************************************************************************/
    void Framebuffer::AttachTexture(const Texture &aTexture, GLenum aTarget,
                                    GLenum aAttachmentType,
                                    GLenum aTextureType) const
    {
        Bind(aTarget);
        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               aAttachmentType,
                               aTextureType,
                               aTexture.GetID(), 0);
    }

    /******************************************************************************/
    void Framebuffer::AttachRenderbuffer(const Renderbuffer &aBuffer,
                                         GLenum aTarget,
                                         GLenum aAttachmentType) const
    {
        Bind(aTarget);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER,
                                  aAttachmentType,
                                  GL_RENDERBUFFER,
                                  aBuffer.GetID());
    }
} // namespace KumaGL
