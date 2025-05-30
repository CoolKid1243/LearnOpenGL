#include "object.h"
#include "mesh.h"
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

// Aliases
using ObjType = Object::Type;

// Static member definitions
std::vector<Object> Object::objects;
bool        Object::meshInitialized[4] = { false };
unsigned int Object::VAOs[4]          = { 0 };
unsigned int Object::VBOs[4]          = { 0 };
unsigned int Object::EBOs[4]          = { 0 };
unsigned int Object::indexCounts[4]   = { 0 };

void Object::Create(
    ObjType type,
    const glm::vec3& color,
    const glm::vec3& scale,
    const glm::vec3& position,
    unsigned int diffuseTex,
    unsigned int specularTex,
    float brightness)
{
    objects.emplace_back(type, color, scale, position, diffuseTex, specularTex, brightness);
}

void Object::RenderAll(Shader& defaultShader, Shader& lightingShader)
{
    // Upload all point-light uniforms
    lightingShader.use();
    int lightIndex = 0;
    for (auto& o : objects) {
        if (o.type == ObjType::PointLight) {
            std::string base = "pointLights[" + std::to_string(lightIndex++) + "]";
            lightingShader.setVec3(base + ".position",  o.position);
            lightingShader.setFloat(base + ".constant", 1.0f);
            lightingShader.setFloat(base + ".linear",   0.09f);
            lightingShader.setFloat(base + ".quadratic",0.032f);
            lightingShader.setVec3(base + ".ambient",   o.color * 0.1f);
            lightingShader.setVec3(base + ".diffuse",   o.color);
            lightingShader.setVec3(base + ".specular",  o.color);
        }
    }

    // Draw geometry (non-lights)
    for (auto& o : objects)
        if (o.type != ObjType::PointLight)
            o.Render(defaultShader, lightingShader);

    // Draw lamp cubes
    for (auto& o : objects)
        if (o.type == ObjType::PointLight)
            o.Render(defaultShader, lightingShader);
}

Object::Object(
    ObjType type,
    const glm::vec3& color,
    const glm::vec3& scale,
    const glm::vec3& position,
    unsigned int diffuseTex,
    unsigned int specularTex,
    float brightness)
  : type(type), color(color), scale(scale), position(position),
    diffuseTex(diffuseTex), specularTex(specularTex), brightness(brightness)
{
    if (!meshInitialized[int(type)])
        InitMesh(type);
}

void Object::InitMesh(ObjType type)
{
    meshInitialized[int(type)] = true;
    unsigned int& VAO = VAOs[int(type)];
    unsigned int& VBO = VBOs[int(type)];
    unsigned int& EBO = EBOs[int(type)];
    unsigned int& IC  = indexCounts[int(type)];

    // Create VAO/VBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if (type == ObjType::Cube || type == ObjType::Plane) {
        // Raw array + count
        const float* verts = (type == ObjType::Cube) ? Mesh::cubeVertices : Mesh::planeVertices;
        IC = (type == ObjType::Cube) ? Mesh::cubeVertexCount : Mesh::planeVertexCount;

        glBufferData(GL_ARRAY_BUFFER, IC * 8 * sizeof(float), verts, GL_STATIC_DRAW);

        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // texcoords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),  (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

    } else if (type == ObjType::Sphere) {
        // Generate sphere data
        std::vector<float> verts;
        std::vector<unsigned int> inds;
        Mesh::GenerateSphereMesh(36, 18, verts, inds);
        IC = (unsigned int)inds.size();

        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

        // EBO
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, IC * sizeof(unsigned int), inds.data(), GL_STATIC_DRAW);

        // positions
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // normals
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // textcoords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

    } else if (type == ObjType::PointLight) {
        IC = Mesh::cubeVertexCount;
        // nothing else bound here
        glBindVertexArray(0);
        return;
    }

    glBindVertexArray(0);
}

void Object::Render(Shader& defaultShader, Shader& lightingShader) const {
    // build model matrix
    glm::mat4 M = glm::translate(glm::mat4(1.0f), position)
                * glm::scale(glm::mat4(1.0f), scale);

    // Render object with no-texture
    if (diffuseTex == 0 && specularTex == 0 && type != ObjType::PointLight) {
        defaultShader.use();
        defaultShader.setMat4("model", M);
        defaultShader.setVec3("objectColor", color);

        glBindVertexArray(VAOs[int(type)]);
        glDrawArrays(GL_TRIANGLES, 0, indexCounts[int(type)]);
    }
    // Render light
    else if (type == ObjType::PointLight) {
        defaultShader.use();
        defaultShader.setMat4("model", M);
        defaultShader.setVec3("objectColor", color * brightness);

        // always draw cube for lamp
        glBindVertexArray(VAOs[int(ObjType::Cube)]);
        glDrawArrays(GL_TRIANGLES, 0, indexCounts[int(ObjType::Cube)]);
    }
    // Texture and lit-object
    else {
        lightingShader.use();
        lightingShader.setMat4("model", M);
        // textures
        if (diffuseTex)  { glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, diffuseTex); }
        if (specularTex) { glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, specularTex); }

        glBindVertexArray(VAOs[int(type)]);
        if (type == ObjType::Sphere) {
            glDrawElements(GL_TRIANGLES, indexCounts[int(type)],
                           GL_UNSIGNED_INT, 0);
        } else {
            glDrawArrays(GL_TRIANGLES, 0, indexCounts[int(type)]);
        }

        // Clear texture
        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, 0);
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindVertexArray(0);
}
