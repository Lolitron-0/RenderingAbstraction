#include "rapch.h"
#include "Mesh.hpp"
#include "MeshImpl.hpp"
#include "ResourceManager.hpp"

namespace Ra
{
    Ref<Ra::Mesh> Mesh::Create(const std::string& path)
    {
        Ref<Mesh> ret = MakeRef<MeshImpl>();

        if (ResourceManager::TryFindAlreadyLoaded<Mesh>(ret, path))
            return ret;

        std::static_pointer_cast<MeshImpl>(ret)->LoadMesh(path);
        return ret;
    }

}