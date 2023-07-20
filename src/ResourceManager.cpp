#include "rapch.h"
#include "ResourceManager.hpp"

namespace Ra
{


    std::vector<std::pair<Ref<void>, Ra::ResourceManager::ResourceType>> ResourceManager::s_LoadedResources{};

}