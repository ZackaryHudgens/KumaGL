#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>

#include "KumaGL.hpp"

namespace KumaGL
{
    class Texture
    {
    public:
        Texture();
        ~Texture();
        Texture(Texture &&aTexture);
        Texture &operator=(Texture &&aTexture);
        Texture(const Texture &aTexture) = delete;
        Texture &operator=(const Texture &aTexture) = delete;

        void Bind(GLenum aTextureUnit = GL_TEXTURE0) const;

        void LoadFromFile(const std::string &aFile, GLint aLoadFormat = GL_RGBA,
                          GLenum aType = GL_UNSIGNED_BYTE);
        void LoadFromData(const void *aData, GLsizei aWidth, GLsizei aHeight,
                          GLint aLoadFormat = GL_RGBA,
                          GLenum aType = GL_UNSIGNED_BYTE);

        void AddSubData(const void *aData, GLint aXOffset, GLint aYOffset,
                        GLsizei aWidth, GLsizei aHeight, GLenum aFormat = GL_RGBA,
                        GLenum aType = GL_UNSIGNED_BYTE);

        void SetParameter(GLenum aParam, GLint aValue) const;
        void SetParameter(GLenum aParam, GLfloat aValue) const;
        void GenerateMipmap() const;

        GLsizei GetWidth() const { return mWidth; }
        GLsizei GetHeight() const { return mHeight; }
        GLuint GetID() const { return mID; }

    private:
        GLsizei mWidth;
        GLsizei mHeight;

        GLuint mID;
        bool mValid;
    };
} // namespace KumaGL

#endif // !TEXTURE_HPP
