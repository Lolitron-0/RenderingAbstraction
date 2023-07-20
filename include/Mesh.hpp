#pragma once
#include "SubMesh.hpp"

namespace Ra
{
    class Mesh
    {
    public:
        virtual const std::vector<SubMesh>& GetSubMeshes() const = 0;
        virtual std::string GetPath() const = 0;
        virtual void ForEashSubmesh(const std::function<void(SubMesh&)>& func) = 0;

        static Ref<Mesh> Create(const std::string& path);
    };
}