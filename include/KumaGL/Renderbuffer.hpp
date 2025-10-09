#ifndef RENDERBUFFER_HPP
#define RENDERBUFFER_HPP

#include "KumaGL.hpp"

#include "GLObject.hpp"

namespace KumaGL
{
  class Renderbuffer : public GLObject
  {
  public:
    Renderbuffer();
    ~Renderbuffer();
    Renderbuffer(Renderbuffer &&aBuffer);
    Renderbuffer &operator=(Renderbuffer &&aBuffer);
    Renderbuffer(const Renderbuffer &) = delete;
    Renderbuffer &operator=(const Renderbuffer &) = delete;

    void Generate();
    void Delete();
    void Bind() const;
    void Unbind() const;

    void SetStorageParameters(GLenum aStorageFormat, GLsizei aWidth,
                              GLsizei aHeight) const;
  };
} // namespace KumaGL

#endif // !RENDERBUFFER_HPP
