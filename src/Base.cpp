#include "rapch.h"
#include "Base.hpp"

namespace Ra
{
    ErrorCallbackFn errorCallback = [](const std::string&) {};

    std::string GetShadersDir()
    {
        return (std::filesystem::path(__FILE__).parent_path().parent_path() / std::filesystem::path("src\\shaders\\")).string();
    }

}