#pragma once
#include "Stopwatch.hpp"
#include <cstddef>

namespace Ra
{
struct RendererStats
{
    int DrawCalls{ 0 };
    std::size_t Indices{ 0 };
    int32_t ScenesPerSecond{ 0 };

    friend class Renderer2D;
    friend class Renderer3D;

private:
    void OnSceneBegin()
    {
        DrawCalls = 0;
        Indices = 0;
    }

    void OnSceneEnd()
    {
        ScenesCounter++;
        if (ScenesPerSecondSW.Elapsed<std::chrono::microseconds>() >= 1'000'000)
        {
            ScenesPerSecond = ScenesCounter;
			ScenesPerSecondSW.Reset();
			ScenesCounter = 0;
        }
    }


    int32_t ScenesCounter{ 0 };
	Stopwatch ScenesPerSecondSW{};
};

} // namespace Ra
