#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include "KumaGL.hpp"

#include "Mat4.hpp"
#include "Vec3.hpp"

namespace KumaGL
{
    class Shader
    {
    public:
        Shader();
        ~Shader();
        Shader(Shader &&aShader);
        Shader &operator=(Shader &&aShader);
        Shader(const Shader &aShader) = delete;
        Shader &operator=(const Shader &aShader) = delete;

        void Use() const;

        void LoadFromFiles(const std::string &aVertexFile,
                           const std::string &aFragmentFile);
        void LoadFromSource(const std::string &aVertexSource,
                            const std::string &aFragmentSource);

        void SetInt(const std::string &aName,
                    int aValue) const;
        void SetFloat(const std::string &aName,
                      float aValue) const;
        void SetVec3(const std::string &aName,
                     const Vec3 &aValue) const;
        void SetMat4(const std::string &aName,
                     const Mat4 &aValue) const;

    private:
        enum class ShaderType
        {
            eVERTEX,
            eFRAGMENT
        };

        GLuint CompileShader(const std::string &aSource,
                             ShaderType aType);
        void LinkProgram(GLuint aVertexID,
                         GLuint aFragmentID);

        GLuint mID;
        bool mValid;
    };
} // namespace KumaGL

#endif // !SHADER_HPP
