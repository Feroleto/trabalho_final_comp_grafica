#pragma once

#include <string>
#include <vector>
#include <map>
#include <stb_image.h>

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

struct MeshEntry {
    unsigned int indexOffset;
    unsigned int indexCount;
    unsigned int materialIndex;
};

class AnimatedModel {
public:
    // carrega modelo base (rig model)
    bool loadModel(const std::string& path);

    // carrega animacao do FBX
    bool loadAnimation(const std::string& name, const std::string& path);

    // carrega as texturas do modelo
    GLuint loadTexture(const std::string& path);
    void loadMaterialTextures();

    // atualiza bones para o tempo atual
    void update(float timeInSeconds, const std::string& animName);

    // retorna a duracao da animacao em segundos (0 se nao existir)
    float getAnimationDuration(const std::string& name) const;

    // desenha o modelo
    void draw();

    // getter para a cena do modelo base
    const aiScene* getModelScene() const { return modelScene; }

    // retorna a matriz global de um bone pelo nome
    glm::mat4 getBoneMatrix(const std::string& boneName) const {
        auto it = boneNameToIndex.find(boneName);
        if (it != boneNameToIndex.end()) {
            return boneMatrices[it->second];
        }
        return glm::mat4(1.0f);
    }
    
    // matrizes dos bones para enviar ao shader
    std::vector<glm::mat4> boneMatrices;

    std::vector<MeshEntry> meshEntries;

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

    // uma textura por material
    std::vector<GLuint> materialTextures;

    // pasta onde ta o arquivo FBX do modelo
    std::string modelDirectory;

    // funções
    void processNode(aiNode* node, const aiScene* scene);
    void processMesh(aiMesh* mesh, const aiScene* scene);
    void setupBuffers();
    void calcBoneTransforms(const std::string& animName, float time,
                            aiNode* node, glm::mat4 parentTransform);
    glm::mat4 aiToGlm(const aiMatrix4x4& m);
};