#pragma once
#include <any>

namespace ra
{
    class RenderingContext
    {
    public:
        /// Struct with all api info (for client logging or whatever)
        struct APIinfo
        {
            std::string Renderer;
            std::string Vendor;
            std::string Version;
        };

        virtual ~RenderingContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        /**
         * @brief Api info getter
         * @return Information about rendering api (vendor, version, etc.)
        */
        virtual APIinfo GetInfo() const = 0;

        /**
         * @brief Factory function for proper context creation (OpenGL context, Vulkan context, etc.)
         * @param windowHandle Native handle of the window to set the current context (e.g. GLFWwindow* for OpenGL) 
         * @return Scope pointer to created context
        */
        static Scope<RenderingContext> Create(std::any windowHandle);

    };
}