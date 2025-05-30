#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>
#include <vector>
#include "../shader.h"

class Object {
public:
    enum class Type { Cube = 0, Plane = 1, Sphere = 2, PointLight = 3 };

    // Create and store a new object
    static void Create(
        Type type,
        const glm::vec3& color,
        const glm::vec3& scale,
        const glm::vec3& position,
        unsigned int diffuseTex = 0,
        unsigned int specularTex = 0,
        float brightness = 1.0f
    );

    // Render all stored objects
    static void RenderAll(Shader& defaultShader, Shader& lightingShader);

    // Public constructor (for internal use)
    Object(
        Type type,
        const glm::vec3& color,
        const glm::vec3& scale,
        const glm::vec3& position,
        unsigned int diffuseTex,
        unsigned int specularTex,
        float brightness
    );

private:
    void Render(Shader& defaultShader, Shader& lightingShader) const;
    static void InitMesh(Type type);

    Type              type;
    glm::vec3         color;
    glm::vec3         scale;
    glm::vec3         position;
    unsigned int      diffuseTex;
    unsigned int      specularTex;
    float             brightness;

    // Static storage and mesh buffers
    static std::vector<Object> objects;
    static bool                meshInitialized[4];
    static unsigned int        VAOs[4];
    static unsigned int        VBOs[4];
    static unsigned int        EBOs[4];
    static unsigned int        indexCounts[4];
};

#endif // OBJECT_H
