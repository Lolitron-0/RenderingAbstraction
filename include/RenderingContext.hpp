#pragma once
#include <any>
#include "Base.hpp"

namespace Ra
{
    /**
     * @brief Environment for renderer 
     * Objects of this class is used to set up a proper rendering context (load correct API version, etc.) and 
     * is responsible for window <-> renderer interaction (such as buffer swapping)
     */
    class RenderingContext
    {
    public:
        /**
         * @brief Struct with all rendering API info (for client logging or whatever)
         * @var APIinfo::NativeVersion 
         * Contains native info object for API (e.g. gladGLversionStruct GLVersion for OpenGL (glad))
         */
        struct APIinfo
        {
            std::string Renderer;
            std::string Vendor;
            std::string Version;
            std::any NativeInfo;
        };

        virtual ~RenderingContext() = default;


        /**
        * @brief Rendering API context initialization
        * @return True if initialization was successful
        */
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        /// Returns true if context has been loaded successfully
        bool IsLoaded() { return m_IsLoaded; }

        /**
         * @brief API info getter
         * @return Information about rendering api (vendor, version, etc.)
        */
        virtual APIinfo GetInfo() const = 0;

        /**
         * @brief Factory method for proper context creation (OpenGL context, Vulkan context, etc.)
         * @param windowHandle Native handle of the window to set the current context (e.g. GLFWwindow* for OpenGL) 
         * @return Scope pointer to created context
         * @note Returns nullptr if no API was specified
        */
        static Scope<RenderingContext> Create(std::any windowHandle);
    protected:
        bool m_IsLoaded{true};
    };
}