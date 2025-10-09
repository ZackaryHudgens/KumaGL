#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include "KumaGL.hpp"

#include "Renderbuffer.hpp"
#include "Texture.hpp"

namespace KumaGL
{
    class Framebuffer
    {
    public:
        Framebuffer();
        ~Framebuffer();
        Framebuffer(Framebuffer &&aBuffer);
        Framebuffer &operator=(Framebuffer &&aBuffer);
        Framebuffer(const Framebuffer &aBuffer) = delete;
        Framebuffer &operator=(const Framebuffer &aBuffer) = delete;

        GLuint GetID() const { return mID; }

        void Bind(GLenum aTarget = GL_FRAMEBUFFER) const;

        void AttachTexture(const Texture &aTexture,
                           GLenum aTarget = GL_FRAMEBUFFER,
                           GLenum aAttachmentType = GL_COLOR_ATTACHMENT0,
                           GLenum aTextureType = GL_TEXTURE_2D) const;
        void AttachRenderbuffer(const Renderbuffer &aBuffer,
                                GLenum aTarget = GL_FRAMEBUFFER,
                                GLenum aAttachmentType = GL_DEPTH_STENCIL_ATTACHMENT) const;

    private:
        GLuint mID;
        bool mValid;
    };
} // namespace KumaGL

#endif // !FRAMEBUFFER_HPP
