#pragma once

#include <Topia.h>
#include <RHIForwardDecl.h>

#include "EngineForwardDecl.h"

#include <EASTL/fixed_vector.h>

namespace topia
{
    enum class EPrimitiveType : u8
    {
        None,
        Plane,
        Cube,
        Sphere,
    };

    class FStaticMesh
    {
        /** Data */
    protected:
        // vertex buffer;
        // index buffer;
        // local positions;
        // local normal, tangnet, bitangent;
        // uv0, uv1, etc;
        // vertex color;
        //

    private:
        FShader* pShader = nullptr;
        FMaterial* pMaterial = nullptr;

        /** Loader */
    public:
        void LoadFromGLTF();
        void LoadPrimitive(EPrimitiveType PrimitiveType);
    };
}
