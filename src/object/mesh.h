#ifndef MESH_H
#define MESH_H

#include <vector>
#include <glm/glm.hpp>

namespace Mesh {
    extern float cubeVertices[];
    extern unsigned int cubeVertexCount;

    extern float planeVertices[];
    extern unsigned int planeVertexCount;

    void GenerateSphereMesh(
        int sectorCount, int stackCount,
        std::vector<float>& outVertices,
        std::vector<unsigned int>& outIndices
    );

    void GenerateCylinderMesh(
        int sectorCount, int heightSegments,   
        std::vector<float>& outVertices,
        std::vector<unsigned int>& outIndices
    );
}

#endif // MESH_H
