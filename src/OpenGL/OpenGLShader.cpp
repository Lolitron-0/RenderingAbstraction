#include "rapch.h"
#include "OpenGL/OpenGLShader.hpp"
#include <glad/glad.h>

namespace Ra
{
    OpenGLShader::OpenGLShader(const std::string& vertPath, const std::string& fragPath)
    {
        auto vertexSource{ LoadSourceFromFile_(vertPath) };
        auto fragSource{ LoadSourceFromFile_(fragPath) };
        Compile_(vertexSource, fragSource);
    }

    OpenGLShader::OpenGLShader(const std::string& shaderPath)
    {
        auto [vertexSource, fragSource] = GetShaderSources_(LoadSourceFromFile_(shaderPath));
        Compile_(vertexSource, fragSource);
    }

    std::pair<std::string, std::string> OpenGLShader::GetShaderSources_(const std::string& data)
    {
        std::pair<std::string, std::string> res;

        const char* typeToken = "#type";
        std::size_t typeTokenLength = strlen(typeToken);
        std::size_t pos = data.find(typeToken, 0);
        while (pos != std::string::npos)
        {
            std::size_t eol = data.find_first_of("\r\n", pos);
            RA_ASSERT(eol != std::string::npos, "Invalid syntax!");
            std::size_t begin = pos + typeTokenLength + 1;
            std::string shaderType = data.substr(begin, eol - begin);
            pos = data.find(typeToken, eol);
            std::size_t nextLinePos = data.find_first_not_of("\r\n", eol);
            RA_ASSERT(nextLinePos != std::string::npos, "Invalid syntax! No next source line!");
            if (shaderType == "vertex")
                res.first = pos == std::string::npos ? data.substr(nextLinePos) : data.substr(nextLinePos, pos - nextLinePos);
            else if (shaderType == "fragment")
                res.second = pos == std::string::npos ? data.substr(nextLinePos) : data.substr(nextLinePos, pos - nextLinePos);
            else
                RA_ASSERT(false, "Invalid shader type!");
        }
        return res;
    }

    void OpenGLShader::Compile_(const std::string& vertexSource, const std::string& fragSource)
    {
        auto vertexSourceData = vertexSource.c_str();
        auto fragmentSourceData = fragSource.c_str();

        auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSourceData, NULL);
        glCompileShader(vertexShader);

        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            RA_ASSERT(false, "Failed to compile vertex shader!  " + std::string(infoLog));
        }

        auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentSourceData, NULL);
        glCompileShader(fragmentShader);

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
            RA_ASSERT(false, "Failed to compile fragment shader!  " + std::string(infoLog));
        }

        m_Handle = glCreateProgram();
        glAttachShader(m_Handle, vertexShader);
        glAttachShader(m_Handle, fragmentShader);
        glLinkProgram(m_Handle);

        glGetProgramiv(m_Handle, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(m_Handle, 512, nullptr, infoLog);
            RA_ASSERT(false, "Failed to link shader!  " + std::string(infoLog));
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    void OpenGLShader::Bind()
    {
        glUseProgram(m_Handle);
    }

    void OpenGLShader::Unbind()
    {
        glUseProgram(0);
    }

    void OpenGLShader::SetInt(const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(m_Handle, name.c_str()), value);
    }

    void OpenGLShader::SetBool(const std::string& name, bool value)
    {
        glUniform1i(glGetUniformLocation(m_Handle, name.c_str()), (int)value);
    }

    void OpenGLShader::SetFloat(const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(m_Handle, name.c_str()), value);
    }

    void OpenGLShader::SetMat3(const std::string& name, const float* value)
    {
        glUniformMatrix3fv(glGetUniformLocation(m_Handle, name.c_str()), 1, GL_FALSE, value);
    }

    void OpenGLShader::SetMat3(const std::string& name, const glm::mat3& mat)
    {
        glUniformMatrix3fv(glGetUniformLocation(m_Handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void OpenGLShader::SetMat4(const std::string& name, const float* value)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Handle, name.c_str()), 1, GL_FALSE, value);
    }

    void OpenGLShader::SetMat4(const std::string& name, const glm::mat4& mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_Handle, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    void OpenGLShader::SetVec3(const std::string& name, const glm::vec3& vec)
    {
        glUniform3fv(glGetUniformLocation(m_Handle, name.c_str()), 1, &vec[0]);
    }

    void OpenGLShader::SetVec3(const std::string& name, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(m_Handle, name.c_str()), x, y, z);
    }

    void OpenGLShader::SetVec4(const std::string& name, const glm::vec4& vec)
    {
        glUniform4fv(glGetUniformLocation(m_Handle, name.c_str()), 1, &vec[0]);
    }

    void OpenGLShader::SetVec4(const std::string& name, float x, float y, float z, float w)
    {
        glUniform4f(glGetUniformLocation(m_Handle, name.c_str()), x, y, z, w);
    }

}
