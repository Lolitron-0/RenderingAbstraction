#pragma once
#include "Base.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Ra
{
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        virtual void SetInt(const std::string name, int value) = 0;
        virtual void SetBool(const std::string name, bool value) = 0;
        virtual void SetFloat(const std::string name, float value) = 0;
        virtual void SetMat4(const std::string name, const float* value) = 0;
        virtual void SetMat4(const std::string name, const glm::mat4& mat) = 0;
        virtual void SetVec3(const std::string name, const glm::vec3& vec) = 0;
        virtual void SetVec3(const std::string name, float x, float y, float z) = 0;

        static Ref<Shader> Create(const std::string& vertPath, const std::string& fragPath);

    protected:
        std::string LoadSourceFromFile_(const std::string& path);
    };
}