#pragma once
#include <any>

namespace ra
{
    class RenderingContext
    {
    public:
        virtual ~RenderingContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;

        static Scope<RenderingContext> Create(std::any windowHandle);
    };
}