#include "KumaGL/Shader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>

namespace KumaGL
{
    /******************************************************************************/
    Shader::Shader() : mID(0),
                       mValid(false)
    {
        mID = glCreateProgram();
        mValid = true;
    }

    /******************************************************************************/
    Shader::~Shader()
    {
        if (mValid)
        {
            glDeleteProgram(mID);
            mValid = false;
        }
    }

    /******************************************************************************/
    Shader::Shader(Shader &&aShader)
    {
        mID = aShader.mID;
        mValid = true;
        aShader.mValid = false;
    }

    /******************************************************************************/
    Shader &Shader::operator=(Shader &&aShader)
    {
        mID = aShader.mID;
        mValid = true;
        aShader.mValid = false;
        return *this;
    }

    /******************************************************************************/
    void Shader::Use() const
    {
        if (mValid)
        {
            glUseProgram(mID);
        }
    }

    /******************************************************************************/
    void Shader::LoadFromFiles(const std::string &aVertexFile,
                               const std::string &aFragmentFile)
    {
        std::ifstream vertexInput, fragmentInput;
        vertexInput.open(aVertexFile);
        fragmentInput.open(aFragmentFile);

        std::stringstream vertexStream, fragmentStream;
        vertexStream << vertexInput.rdbuf();
        fragmentStream << fragmentInput.rdbuf();

        vertexInput.close();
        fragmentInput.close();

        LoadFromSource(vertexStream.str(), fragmentStream.str());
    }

    /******************************************************************************/
    void Shader::LoadFromSource(const std::string &aVertexSource,
                                const std::string &aFragmentSource)
    {
        // First, compile the shaders.
        GLuint vertexID = CompileShader(aVertexSource, ShaderType::eVERTEX);
        GLuint fragmentID = CompileShader(aFragmentSource, ShaderType::eFRAGMENT);

        // Then, create and link the shader program.
        LinkProgram(vertexID, fragmentID);

        // Finally, delete the shaders since they have been linked into the
        // program and are no longer needed.
        glDeleteShader(vertexID);
        glDeleteShader(fragmentID);
    }

    /******************************************************************************/
    void Shader::SetInt(const std::string &aName, int aValue) const
    {
        int loc = glGetUniformLocation(mID, aName.c_str());
        glUniform1i(loc, aValue);
    }

    /******************************************************************************/
    void Shader::SetFloat(const std::string &aName, float aValue) const
    {
        int loc = glGetUniformLocation(mID, aName.c_str());
        glUniform1f(loc, aValue);
    }

    /******************************************************************************/
    void Shader::SetVec3(const std::string &aName, const Vec3 &aValue) const
    {
        int loc = glGetUniformLocation(mID, aName.c_str());
        glUniform3fv(loc, 1, &aValue.x);
    }

    /******************************************************************************/
    void Shader::SetMat4(const std::string &aName, const Mat4 &aValue) const
    {
        int loc = glGetUniformLocation(mID, aName.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, aValue.GetData());
    }

    /******************************************************************************/
    GLuint Shader::CompileShader(const std::string &aSource,
                                 ShaderType aType)
    {
        GLuint id;
        const char *source = aSource.c_str();

        switch (aType)
        {
        case ShaderType::eVERTEX:
        {
            id = glCreateShader(GL_VERTEX_SHADER);
            break;
        }
        case ShaderType::eFRAGMENT:
        {
            id = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        }
        }

        glShaderSource(id, 1, &source, NULL);
        glCompileShader(id);

        // Check for compile errors.
        int success;
        char infoLog[512];
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(id, 512, NULL, infoLog);
            std::cout << "Error compiling shader!\n"
                      << infoLog << std::endl;
        }

        return id;
    }

    /******************************************************************************/
    void Shader::LinkProgram(GLuint aVertexID, GLuint aFragmentID)
    {
        glAttachShader(mID, aVertexID);
        glAttachShader(mID, aFragmentID);
        glLinkProgram(mID);

        // Check for linking errors.
        int success;
        char infoLog[512];
        glGetProgramiv(mID, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(mID, 512, NULL, infoLog);
            std::cout << "Error linking program!\n"
                      << infoLog << std::endl;
        }
    }
} // namespace KumaGL
