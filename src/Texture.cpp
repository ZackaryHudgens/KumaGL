#include "KumaGL/Texture.hpp"
#include "KumaGL/GLObject.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace KumaGL
{
    /******************************************************************************/
    Texture::Texture() : mWidth(0),
                         mHeight(0),
                         mID(0),
                         mValid(false)
    {
        glGenTextures(1, &mID);
        mValid = true;
    }

    /******************************************************************************/
    Texture::~Texture()
    {
        if (mValid)
        {
            glDeleteTextures(1, &mID);
            mValid = false;
        }
    }

    /******************************************************************************/
    Texture::Texture(Texture &&aTexture)
    {
        mWidth = aTexture.mWidth;
        mHeight = aTexture.mHeight;
        mID = aTexture.mID;
        mValid = true;
        aTexture.mValid = false;
    }

    /******************************************************************************/
    Texture &Texture::operator=(Texture &&aTexture)
    {
        mWidth = aTexture.mWidth;
        mHeight = aTexture.mHeight;
        mID = aTexture.mID;
        mValid = true;
        aTexture.mValid = false;
        return *this;
    }

    /******************************************************************************/
    void Texture::Bind(GLenum aTextureUnit) const
    {
        if (mValid)
        {
            glActiveTexture(aTextureUnit);
            glBindTexture(GL_TEXTURE_2D, mID);
            glActiveTexture(GL_TEXTURE0);
        }
    }

    /******************************************************************************/
    void Texture::LoadFromFile(const std::string &aFile, GLint aLoadFormat,
                               GLenum aType)
    {
        stbi_set_flip_vertically_on_load(true);

        int channels;
        auto data = stbi_load(aFile.c_str(), &mWidth, &mHeight, &channels, 0);

        LoadFromData(data, mWidth, mHeight, aLoadFormat, aType);

        stbi_image_free(data);
    }

    /******************************************************************************/
    void Texture::LoadFromData(const void *aData, GLsizei aWidth, GLsizei aHeight,
                               GLint aLoadFormat, GLenum aType)
    {
        mWidth = aWidth;
        mHeight = aHeight;

        Bind();
        glTexImage2D(GL_TEXTURE_2D, 0, aLoadFormat, mWidth, mHeight, 0, aLoadFormat,
                     aType, aData);
    }

    /******************************************************************************/
    void Texture::AddSubData(const void *aData, GLint aXOffset, GLint aYOffset,
                             GLsizei aWidth, GLsizei aHeight, GLenum aFormat,
                             GLenum aType)
    {
        Bind();
        glTexSubImage2D(GL_TEXTURE_2D, 0, aXOffset, aYOffset, aWidth, aHeight,
                        aFormat, aType, aData);
    }

    /******************************************************************************/
    void Texture::SetParameter(GLenum aParam, GLint aValue) const
    {
        Bind();
        glTexParameteri(GL_TEXTURE_2D, aParam, aValue);
    }

    /******************************************************************************/
    void Texture::SetParameter(GLenum aParam, GLfloat aValue) const
    {
        Bind();
        glTexParameterf(GL_TEXTURE_2D, aParam, aValue);
    }

    /******************************************************************************/
    void Texture::GenerateMipmap() const
    {
        Bind();
        glGenerateMipmap(GL_TEXTURE_2D);
    }
} // namespace KumaGL
