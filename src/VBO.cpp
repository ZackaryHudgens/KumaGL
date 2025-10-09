#include "KumaGL/VBO.hpp"
#include "KumaGL/GLObject.hpp"

#include <algorithm>

namespace KumaGL
{
  /******************************************************************************/
  VBO::VBO() { Generate(); }

  /******************************************************************************/
  VBO::~VBO() { Delete(); }

  /******************************************************************************/
  VBO::VBO(VBO &&aBuffer) : GLObject(std::move(aBuffer)) {}

  /******************************************************************************/
  VBO &VBO::operator=(VBO &&aBuffer)
  {
    Delete();
    GLObject::operator=(std::move(aBuffer));
    return *this;
  }

  /******************************************************************************/
  void VBO::Generate()
  {
    if (!mID)
    {
      glGenBuffers(1, &mID);
    }
  }

  /******************************************************************************/
  void VBO::Delete()
  {
    if (mID)
    {
      glDeleteBuffers(1, &mID);
      mID = 0;
    }
  }

  /******************************************************************************/
  void VBO::Bind(GLenum aTarget) const { glBindBuffer(aTarget, mID); }

  /******************************************************************************/
  void VBO::Unbind(GLenum aTarget) const { glBindBuffer(aTarget, 0); }

  /******************************************************************************/
  void VBO::CopyData(GLenum aTarget,
                     GLsizeiptr aSize,
                     const void *aData,
                     GLenum aUsage) const
  {
    Bind(aTarget);
    glBufferData(aTarget, aSize, aData, aUsage);
    Unbind(aTarget);
  }

  /******************************************************************************/
  void VBO::CopySubData(GLenum aTarget,
                        GLintptr aOffset,
                        GLsizeiptr aSize,
                        const void *aData) const
  {
    Bind(aTarget);
    glBufferSubData(aTarget, aOffset, aSize, aData);
    Unbind(aTarget);
  }
} // namespace KumaGL
