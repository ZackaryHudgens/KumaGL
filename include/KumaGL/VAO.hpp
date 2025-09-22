#ifndef VAO_HPP
#define VAO_HPP

#include "GLObject.hpp"

#include "VBO.hpp"

namespace KumaGL
{
    class VAO : public GLObject
    {
    public:
        VAO();
        ~VAO();
        VAO(VAO &&);
        VAO &operator=(VAO &&);
        VAO(const VAO &) = delete;
        VAO &operator=(const VAO &) = delete;

        void Generate();
        void Delete();
        void Bind() const;
        void Unbind() const;

        void ConfigureVertexAttribute(VBO &aBuffer, GLuint aIndex, GLint aSize,
                                      GLenum aType, GLboolean aNormalized,
                                      GLsizei aStride, const void *aOffset) const;
        void ConfigureVertexAttributeWithDivisor(VBO &aBuffer, GLuint aIndex,
                                                 GLint aSize, GLenum aType,
                                                 GLboolean aNormalized,
                                                 GLsizei aStride, const void *aOffset,
                                                 GLuint aDivisor) const;

        void SetElementBuffer(VBO &aBuffer) const;
    };
} // namespace KumaGL

#endif // !VAO_HPP
