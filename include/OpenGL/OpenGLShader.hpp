#pragma once
#include "Shader.hpp"

namespace Ra
{
    /// Implementation for OpenGL GLSL shader
    class OpenGLShader : public Shader
    {
    public:
        OpenGLShader(const std::string& vertPath, const std::string& fragPath);
        OpenGLShader(const std::string& shaderPath);

        void Bind() override;
        void Unbind() override;


        void SetInt(const   std::string& name, int value) override;
        void SetBool(const  std::string& name, bool value) override;
        void SetFloat(const std::string& name, float value) override;
        void SetMat3(const  std::string& name, const float* value) override;
        void SetMat3(const  std::string& name, const glm::mat3& mat) override;
        void SetMat4(const  std::string& name, const float* value) override;
        void SetMat4(const  std::string& name, const glm::mat4& mat) override;
        void SetVec3(const  std::string& name, const glm::vec3& vec) override;
        void SetVec3(const  std::string& name, float x, float y, float z) override;
        void SetVec4(const  std::string& name, const glm::vec4& vec) override;
        void SetVec4(const  std::string& name, float x, float y, float z, float w) override;
    private:
        std::pair<std::string, std::string> GetShaderSources_(const std::string& data);
        void Compile_(const std::string& vertexSource, const std::string& fragSource);

        RendererId m_Handle;
    };
}