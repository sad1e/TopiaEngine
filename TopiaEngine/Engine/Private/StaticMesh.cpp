#include <StaticMesh.h>

namespace topia
{
    /** Load nothing, initialize relative variables to default. */
    void LoadPrimitiveEmpty()
    {
    }

    void LoadPrimitivePlane()
    {
    }

    void LoadPrimitiveCube()
    {
    }

    /** TODO: Load cube for now. */
    void LoadPrimitiveSphere()
    {
        LoadPrimitiveCube();
    }

    void FStaticMesh::LoadPrimitive(EPrimitiveType PrimitiveType)
    {
        switch (PrimitiveType)
        {
        case EPrimitiveType::Plane:
            LoadPrimitivePlane();
            break;
        case EPrimitiveType::Cube:
            LoadPrimitiveCube();
            break;
        case EPrimitiveType::Sphere:
            LoadPrimitiveSphere();
            break;
        case EPrimitiveType::None:
            LoadPrimitiveEmpty();
            break;
        default:
            break;
        }
    }

    void FStaticMesh::LoadFromGLTF()
    {
    }
}
