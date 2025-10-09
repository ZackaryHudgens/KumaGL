#ifndef RENDERBUFFER_HPP
#define RENDERBUFFER_HPP

#include "KumaGL.hpp"

namespace KumaGL
{
    class Renderbuffer
    {
    public:
        Renderbuffer();
        ~Renderbuffer();
        Renderbuffer(Renderbuffer &&aBuffer);
        Renderbuffer &operator=(Renderbuffer &&aBuffer);
        Renderbuffer(const Renderbuffer &) = delete;
        Renderbuffer &operator=(const Renderbuffer &) = delete;

        GLuint GetID() const { return mID; }

        void Bind() const;

        void SetStorageParameters(GLenum aStorageFormat,
                                  GLsizei aWidth,
                                  GLsizei aHeight) const;

    private:
        GLuint mID;
        bool mValid;
    };
} // namespace KumaGL

#endif // !RENDERBUFFER_HPP
