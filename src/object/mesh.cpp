#include "mesh.h"
#include <cmath>

namespace Mesh {
    float cubeVertices[] = {
        // positions          // normals           // texcoords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        // front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
        // left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f, 0.0f,   1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f, 0.0f,   1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f, 0.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f, 0.0f,   0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f, 0.0f,   1.0f, 0.0f,
        // right face
        0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 0.0f,    1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 0.0f,    0.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f,  0.0f, 0.0f,    1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f,  0.0f, 0.0f,    0.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f,  0.0f, 0.0f,    1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f,  0.0f, 0.0f,    0.0f, 0.0f,
        // bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,    1.0f, 1.0f,
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,    1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,    0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,    0.0f, 1.0f,
        // top face
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,     1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,     1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,     0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,     0.0f, 0.0f
    };
    unsigned int cubeVertexCount = 36;

    float planeVertices[] = {
        // positions            // normals           // texcoords
        -5.0f,  0.0f,  5.0f,    0.0f, 1.0f, 0.0f,   0.0f, 0.0f,
        5.0f,  0.0f,  5.0f,    0.0f, 1.0f, 0.0f,   5.0f, 0.0f,
        -5.0f,  0.0f, -5.0f,    0.0f, 1.0f, 0.0f,   0.0f, 5.0f,

        -5.0f,  0.0f, -5.0f,    0.0f, 1.0f, 0.0f,   0.0f, 5.0f,
        5.0f,  0.0f,  5.0f,    0.0f, 1.0f, 0.0f,   5.0f, 0.0f,
        5.0f,  0.0f, -5.0f,    0.0f, 1.0f, 0.0f,   5.0f, 5.0f
    };
    unsigned int planeVertexCount = 6;

    void GenerateSphereMesh(int sectorCount, int stackCount,
                            std::vector<float>& vertices,
                            std::vector<unsigned int>& indices)
    {
        const float PI = std::acos(-1.0f);
        const float radius = 0.5f;

        vertices.clear();
        indices.clear();

        // generate vertices (pos + normal packed into 6 floats per vertex)
        for (unsigned int y = 0; y <= stackCount; ++y) {
            for (unsigned int x = 0; x <= sectorCount; ++x) {
                float xSegment = (float)x / sectorCount;
                float ySegment = (float)y / stackCount;
                float xPos = radius * std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = radius * std::cos(ySegment * PI);
                float zPos = radius * std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                // position
                vertices.push_back(xPos);
                vertices.push_back(yPos);
                vertices.push_back(zPos);
                // normal (same as position for a sphere)
                vertices.push_back(xPos / radius);
                vertices.push_back(yPos / radius);
                vertices.push_back(zPos / radius);
            }
        }

        // generate CCW indices
        for (unsigned int y = 0; y < stackCount; ++y) {
            for (unsigned int x = 0; x < sectorCount; ++x) {
                unsigned int i0 =  y      * (sectorCount + 1) + x;
                unsigned int i1 = (y + 1) * (sectorCount + 1) + x;
                unsigned int i2 = (y + 1) * (sectorCount + 1) + x + 1;
                unsigned int i3 =  y      * (sectorCount + 1) + x + 1;

                // first triangle
                indices.push_back(i0);
                indices.push_back(i1);
                indices.push_back(i2);
                // second triangle
                indices.push_back(i0);
                indices.push_back(i2);
                indices.push_back(i3);
            }
        }
    }
}
