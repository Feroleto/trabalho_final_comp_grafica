#pragma once

#include <string>
#include <vector>
#include <map>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


// vertice do modelo animado
struct AnimVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
    int   boneIDs[4]     = {0, 0, 0, 0};
    float boneWeights[4] = {0, 0, 0, 0};
};

// animacao carregada do FBX
struct Animation {
    std::string name;
    float duration;
    float ticksPerSecond;
    const aiScene* scene;
};

class AnimatedModel {
public:
    // carrega modelo base (rig model)
    bool loadModel(const std::string& path);

    // carrega animacao do FBX
    bool loadAnimation(const std::string& name, const std::string& path);

    // atualiza bones para o tempo atual
    void update(float timeInSeconds, const std::string& animName);

    // desenha o modelo
    void draw();

    // matrizes dos bones para enviar ao shader
    std::vector<glm::mat4> boneMatrices;

private:
    // dados do modelo
    std::vector<AnimVertex> vertices;
    std::vector<unsigned int> indices;
    GLuint VAO, VBO, EBO;

    // dados dos bones
    std::map<std::string, int> boneNameToIndex;
    std::vector<glm::mat4> boneOffsets;
    int boneCount = 0;

    // animações carregadas
    std::map<std::string, Animation> animations;

    // importadores de assimp
    Assimp::Importer modelImporter;
    std::map<std::string, Assimp::Importer*> animImporters;

    // cena do modelo base (rig model)
    const aiScene* modelScene = nullptr;

    // funções
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    void setupBuffers();
    void calcBoneTransforms(const std::string& animName, float time,
                            aiNode* node, glm::mat4 parentTransform);
    glm::mat4 aiToGlm(const aiMatrix4x4& m);
};