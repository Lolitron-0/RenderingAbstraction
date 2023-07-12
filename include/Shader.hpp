#pragma once
#include "Base.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Ra
{
    /// Shader abstraction class
    class Shader
    {
    public:
        virtual ~Shader() = default;

        virtual void Bind() = 0;
        virtual void Unbind() = 0;

        /// Uniform int variable setter
        virtual void SetInt(const std::string name, int value) = 0;
        /// Uniform bool variable setter
        virtual void SetBool(const std::string name, bool value) = 0;
        /// Uniform float variable setter
        virtual void SetFloat(const std::string name, float value) = 0;
        /// Uniform 4x4 matrix variable setter
        virtual void SetMat4(const std::string name, const float* value) = 0;
        /// Uniform 4x4 matrix variable setter
        virtual void SetMat4(const std::string name, const glm::mat4& mat) = 0;
        /// Uniform vec3 variable setter
        virtual void SetVec3(const std::string name, const glm::vec3& vec) = 0;
        /// Uniform vec3 variable setter
        virtual void SetVec3(const std::string name, float x, float y, float z) = 0;
        /// Uniform vec4 variable setter
        virtual void SetVec4(const std::string name, const glm::vec4& vec) = 0;
        /// Uniform vec4 variable setter
        virtual void SetVec4(const std::string name, float x, float y, float z, float w) = 0;

        /**
         * @brief Factory static method to create rendering API independent shaders
         * @param vertPath Path to vertex source file
         * @param fragPath Path to fragment source file
         * @return Ref-counted object with created shader. Or nullptr if rendering API is unknown/unset
        */
        static Scope<Shader> Create(const std::string& vertPath, const std::string& fragPath);
        /**
         * @brief Factory static method to create rendering API independent shaders
         * @param shaderPath Path to source file
         * @return Ref-counted object with created shader. Or nullptr if rendering API is unknown/unset
        */
        static Scope<Shader> Create(const std::string& shaderPath);

    protected:
        std::string LoadSourceFromFile_(const std::string& path);
    };
}