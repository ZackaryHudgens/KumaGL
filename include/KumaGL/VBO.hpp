#ifndef VBO_HPP
#define VBO_HPP

#include "GLObject.hpp"

namespace KumaGL
{
  class VBO : public GLObject
  {
  public:
    VBO();
    ~VBO();
    VBO(VBO &&);
    VBO &operator=(VBO &&);
    VBO(const VBO &) = delete;
    VBO &operator=(const VBO &) = delete;

    void Generate();
    void Delete();
    void Bind(GLenum aTarget) const;
    void Unbind(GLenum aTarget) const;

    void CopyData(GLenum aTarget,
                  GLsizeiptr aSize,
                  const void *aData,
                  GLenum aUsage) const;
    void CopySubData(GLenum aTarget,
                     GLintptr aOffset,
                     GLsizeiptr aSize,
                     const void *aData) const;
  };
} // namespace KumaGL

#endif // !VBO_HPP
