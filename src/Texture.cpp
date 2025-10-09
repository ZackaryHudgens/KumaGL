#include "KumaGL/Texture.hpp"
#include "KumaGL/GLObject.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace KumaGL
{
  /******************************************************************************/
  Texture::Texture() { Generate(); }

  /******************************************************************************/
  Texture::~Texture() { Delete(); }

  /******************************************************************************/
  Texture::Texture(Texture &&aTexture) : GLObject(std::move(aTexture))
  {
    mWidth = aTexture.mWidth;
    mHeight = aTexture.mHeight;
  }

  /******************************************************************************/
  Texture &Texture::operator=(Texture &&aTexture)
  {
    Delete();
    GLObject::operator=(std::move(aTexture));
    mWidth = aTexture.mWidth;
    mHeight = aTexture.mHeight;

    return *this;
  }

  /******************************************************************************/
  void Texture::Generate()
  {
    if (!mID)
    {
      glGenTextures(1, &mID);
    }
  }

  /******************************************************************************/
  void Texture::Delete()
  {
    if (mID)
    {
      glDeleteTextures(1, &mID);
      mID = 0;
    }
  }

  /******************************************************************************/
  void Texture::Bind(GLenum aTextureUnit) const
  {
    glActiveTexture(aTextureUnit);
    glBindTexture(GL_TEXTURE_2D, mID);
    glActiveTexture(GL_TEXTURE0);
  }

  /******************************************************************************/
  void Texture::Unbind() const { glBindTexture(GL_TEXTURE_2D, 0); }

  /******************************************************************************/
  void Texture::LoadFromFile(const std::string &aFile,
                             GLint aLoadFormat,
                             GLenum aType)
  {
    stbi_set_flip_vertically_on_load(true);

    int channels;
    auto data = stbi_load(aFile.c_str(), &mWidth, &mHeight, &channels, 0);

    LoadFromData(data, mWidth, mHeight, aLoadFormat, aType);

    stbi_image_free(data);
  }

  /******************************************************************************/
  void Texture::LoadFromData(const void *aData,
                             GLsizei aWidth,
                             GLsizei aHeight,
                             GLint aLoadFormat,
                             GLenum aType)
  {
    mWidth = aWidth;
    mHeight = aHeight;

    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, aLoadFormat, mWidth, mHeight, 0, aLoadFormat,
                 aType, aData);
    Unbind();
  }

  /******************************************************************************/
  void Texture::AddSubData(const void *aData,
                           GLint aXOffset,
                           GLint aYOffset,
                           GLsizei aWidth,
                           GLsizei aHeight,
                           GLenum aFormat,
                           GLenum aType)
  {
    Bind();
    glTexSubImage2D(GL_TEXTURE_2D, 0, aXOffset, aYOffset, aWidth, aHeight,
                    aFormat, aType, aData);
    Unbind();
  }

  /******************************************************************************/
  void Texture::SetParameter(GLenum aParam, GLint aValue) const
  {
    Bind();
    glTexParameteri(GL_TEXTURE_2D, aParam, aValue);
    Unbind();
  }

  /******************************************************************************/
  void Texture::SetParameter(GLenum aParam, GLfloat aValue) const
  {
    Bind();
    glTexParameterf(GL_TEXTURE_2D, aParam, aValue);
    Unbind();
  }

  /******************************************************************************/
  void Texture::GenerateMipmap() const
  {
    Bind();
    glGenerateMipmap(GL_TEXTURE_2D);
    Unbind();
  }
} // namespace KumaGL
