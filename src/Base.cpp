#include "rapch.h"
#include "Base.hpp"

namespace Ra
{
    CallbackFn errorCallback = [](const std::string&) {};
    CallbackFn logCallback = [](const std::string&) {};

    std::string GetShadersDir()
    {
        return (std::filesystem::path(__FILE__).parent_path().parent_path() / std::filesystem::path("src\\shaders\\")).string();
    }

    std::string GetMeshesDir()
    {
        return (std::filesystem::path(__FILE__).parent_path().parent_path() / std::filesystem::path("meshes\\")).string();
    }

}