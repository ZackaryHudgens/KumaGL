#ifndef GLOBJECT_HPP
#define GLOBJECT_HPP

#include "KumaGL.hpp"

namespace KumaGL
{
  class GLObject
  {
  public:
    GLObject() = default;
    virtual ~GLObject() = default;
    GLObject(GLObject &&);
    GLObject &operator=(GLObject &&);
    GLObject(const GLObject &) = delete;
    GLObject &operator=(const GLObject &) = delete;

    GLuint GetID() const { return mID; }

  protected:
    GLuint mID{0};
  };
} // namespace KumaGL

#endif // !GLOBJECT_HPP
