#pragma once
#include "Texture.hpp"
#include "Mesh.hpp"

namespace Ra
{

    class ResourceManager
    {
        enum class ResourceType : std::uint8_t
        {
            Unknown,
            Texture,
            Mesh,
        };

    public:
        /**
         * @brief Checks if resource has been loaded
         * @param allocatedResource Resource to validate, it will be changed to already loaded resource if such is found
         * @param resourcePath path to resource
         * @tparam Resource type (e.g. Mesh, Texture)
         * @return If resource was already loaded and given pointer is altered.
         * @note Given resource should be allocated and if it is new (not loaded) should be loaded via same pointer
        */
        template<class T>
        static bool TryFindAlreadyLoaded(Ref<T>& allocatedResource, const std::string& resourcePath)
        {
            RA_ASSERT(allocatedResource, "Resource should be allocated before validation!");
            ResourceType type = GetTextureType_<T>();
            decltype(s_LoadedResources)::iterator sameIt;
            if ((sameIt = std::find_if(s_LoadedResources.begin(), s_LoadedResources.end(),
                [&resourcePath, &type](const std::pair<Ref<void>, ResourceType>& el) -> bool
                { 
                    if (el.second == type)
                        return (std::static_pointer_cast<T>(el.first)->GetPath() == resourcePath); 
                    return false;
                })) != s_LoadedResources.end())
            {
                allocatedResource = std::static_pointer_cast<T>(sameIt->first);
                return true;
            }
            s_LoadedResources.push_back(std::make_pair(std::static_pointer_cast<void>(allocatedResource), type));
            return false;
        }
    private:
        template<class T>
        static constexpr ResourceManager::ResourceType GetTextureType_()
        {
            if (typeid(T) == typeid(Texture))
                return ResourceManager::ResourceType::Texture;
            else if (typeid(T) == typeid(Mesh))
                return ResourceManager::ResourceType::Mesh;
            return ResourceManager::ResourceType::Unknown;
        }

        static std::vector<std::pair<Ref<void>, ResourceType>> s_LoadedResources;
    };

}