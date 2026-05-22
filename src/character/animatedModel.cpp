#include "AnimatedModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstdio>

glm::mat4 AnimatedModel::aiToGlm(const aiMatrix4x4& m) {
    return glm::transpose(glm::make_mat4(&m.a1));
}

bool AnimatedModel::loadModel(const std::string& path) {
    modelScene = modelImporter.ReadFile(path,
        aiProcess_Triangulate |
        aiProcess_FlipUVs     |
        aiProcess_GenNormals
    );

    if (!modelScene || !modelScene->mRootNode) {
        printf("Erro ao carregar modelo: %s\n", modelImporter.GetErrorString());
        return false;
    }

    processNode(modelScene->mRootNode, modelScene);
    setupBuffers();

    modelDirectory = path.substr(0, path.find_last_of("/\\"));
    loadMaterialTextures();

    boneMatrices.resize(boneCount, glm::mat4(1.0f));
    printf("Modelo carregado: %d bones, %zu vertices\n", boneCount, vertices.size());
    return true;
}

bool AnimatedModel::loadAnimation(const std::string& name, const std::string& path) {
    Assimp::Importer* importer = new Assimp::Importer();
    const aiScene* scene = importer->ReadFile(path,
        aiProcess_Triangulate
    );

    if (!scene || scene->mNumAnimations == 0) {
        printf("Erro ao carregar animacao '%s'\n", name.c_str());
        delete importer;
        return false;
    }

    Animation anim;
    anim.name           = name;
    anim.scene          = scene;
    anim.duration       = (float)scene->mAnimations[0]->mDuration;
    anim.ticksPerSecond = (float)scene->mAnimations[0]->mTicksPerSecond;
    if (anim.ticksPerSecond == 0) anim.ticksPerSecond = 25.0f;

    animations[name]    = anim;
    animImporters[name] = importer;

    printf("Animacao '%s' carregada: %.1f ticks @ %.1f tps\n",
           name.c_str(), anim.duration, anim.ticksPerSecond);
    return true;
}

void AnimatedModel::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        processMesh(mesh, scene);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

void AnimatedModel::processMesh(aiMesh* mesh, const aiScene* scene) {
    unsigned int baseVertex = (unsigned int)vertices.size();
    unsigned int baseIndex = (unsigned int)indices.size();

    // Vértices
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        AnimVertex v;
        v.position  = glm::vec3(mesh->mVertices[i].x,
                                mesh->mVertices[i].y,
                                mesh->mVertices[i].z);
        v.normal    = mesh->HasNormals()
                    ? glm::vec3(mesh->mNormals[i].x,
                                mesh->mNormals[i].y,
                                mesh->mNormals[i].z)
                    : glm::vec3(0, 1, 0);
        v.texCoords = mesh->mTextureCoords[0]
                    ? glm::vec2(mesh->mTextureCoords[0][i].x,
                                mesh->mTextureCoords[0][i].y)
                    : glm::vec2(0, 0);
        vertices.push_back(v);
    }

    // Índices
    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace& face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(baseVertex + face.mIndices[j]);
    }

    // Bones
    for (unsigned int i = 0; i < mesh->mNumBones; i++) {
        aiBone* bone = mesh->mBones[i];
        std::string boneName(bone->mName.C_Str());

        if (boneNameToIndex.find(boneName) == boneNameToIndex.end()) {
            boneNameToIndex[boneName] = boneCount;
            boneOffsets.push_back(aiToGlm(bone->mOffsetMatrix));
            boneCount++;
        }

        int boneIndex = boneNameToIndex[boneName];
        for (unsigned int j = 0; j < bone->mNumWeights; j++) {
            unsigned int vertexID = baseVertex + bone->mWeights[j].mVertexId;
            float weight          = bone->mWeights[j].mWeight;

            // Adiciona nos primeiros 4 slots disponíveis
            for (int k = 0; k < 4; k++) {
                if (vertices[vertexID].boneWeights[k] == 0.0f) {
                    vertices[vertexID].boneIDs[k]     = boneIndex;
                    vertices[vertexID].boneWeights[k] = weight;
                    break;
                }
            }
        }
    }

    MeshEntry entry;
    entry.indexOffset = baseIndex;
    entry.indexCount = mesh->mNumFaces * 3;
    entry.materialIndex = mesh->mMaterialIndex;
    meshEntries.push_back(entry);
}

void AnimatedModel::setupBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(AnimVertex),
                 vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    // position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          sizeof(AnimVertex), (void*)offsetof(AnimVertex, position));
    // normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
                          sizeof(AnimVertex), (void*)offsetof(AnimVertex, normal));
    // texcoords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,
                          sizeof(AnimVertex), (void*)offsetof(AnimVertex, texCoords));
    // boneIDs
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT,
                           sizeof(AnimVertex), (void*)offsetof(AnimVertex, boneIDs));
    // boneWeights
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE,
                          sizeof(AnimVertex), (void*)offsetof(AnimVertex, boneWeights));

    glBindVertexArray(0);
}

void AnimatedModel::update(float timeInSeconds, const std::string& animName) {
    if (animations.find(animName) == animations.end()) return;

    Animation& anim  = animations[animName];
    float ticks      = timeInSeconds * anim.ticksPerSecond;
    float animTime   = fmod(ticks, anim.duration);

    boneMatrices.assign(boneCount, glm::mat4(1.0f));
    glm::mat4 identity(1.0f);
    glm::mat4 globalInverse = glm::inverse(aiToGlm(modelScene->mRootNode->mTransformation));

    calcBoneTransforms(animName, animTime,
                       anim.scene->mRootNode, identity);
}

void AnimatedModel::calcBoneTransforms(const std::string& animName,
                                        float animTime,
                                        aiNode* node,
                                        glm::mat4 parentTransform) {
    std::string nodeName(node->mName.C_Str());
    Animation& anim     = animations[animName];
    glm::mat4 nodeTransform = aiToGlm(node->mTransformation);

    // Procura o canal de animação para esse node
    aiAnimation* aiAnim = anim.scene->mAnimations[0];
    for (unsigned int i = 0; i < aiAnim->mNumChannels; i++) {
        aiNodeAnim* channel = aiAnim->mChannels[i];
        if (std::string(channel->mNodeName.C_Str()) != nodeName) continue;
        // troquei mName por mNodeName, que é o que tem na struct aiNodeAnim

        // Posição
        glm::vec3 position(0);
        if (channel->mNumPositionKeys > 0) {
            unsigned int idx = 0;
            for (unsigned int j = 0; j + 1 < channel->mNumPositionKeys; j++) {
                if (animTime < (float)channel->mPositionKeys[j + 1].mTime) {
                    idx = j; break;
                }
            }
            auto& p = channel->mPositionKeys[idx].mValue;
            position = glm::vec3(p.x, p.y, p.z);
        }

        // Rotação
        glm::mat4 rotation(1.0f);
        if (channel->mNumRotationKeys > 0) {
            unsigned int idx = 0;
            for (unsigned int j = 0; j + 1 < channel->mNumRotationKeys; j++) {
                if (animTime < (float)channel->mRotationKeys[j + 1].mTime) {
                    idx = j; break;
                }
            }
            aiMatrix3x3 rotMat = channel->mRotationKeys[idx].mValue.GetMatrix();
            aiMatrix4x4 rotMat4(rotMat);
            rotation = aiToGlm(rotMat4);
        }

        // Escala
        glm::vec3 scale(1);
        if (channel->mNumScalingKeys > 0) {
            unsigned int idx = 0;
            for (unsigned int j = 0; j + 1 < channel->mNumScalingKeys; j++) {
                if (animTime < (float)channel->mScalingKeys[j + 1].mTime) {
                    idx = j; break;
                }
            }
            auto& s = channel->mScalingKeys[idx].mValue;
            scale = glm::vec3(s.x, s.y, s.z);
        }

        nodeTransform = glm::translate(glm::mat4(1.0f), position)
                      * rotation
                      * glm::scale(glm::mat4(1.0f), scale);
        break;
    }

    glm::mat4 globalTransform = parentTransform * nodeTransform;

    if (boneNameToIndex.count(nodeName)) {
        int idx = boneNameToIndex[nodeName];
        glm::mat4 globalInverse = glm::inverse(
            aiToGlm(modelScene->mRootNode->mTransformation));
        boneMatrices[idx] = globalInverse * globalTransform * boneOffsets[idx];
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        calcBoneTransforms(animName, animTime, node->mChildren[i], globalTransform);
    }
}

GLuint AnimatedModel::loadTexture(const std::string& path) {
    GLuint texID;
    glGenTextures(1, &texID);

    int w, h, channels;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 0);
    if (data) {
        GLenum format = (channels == 4) ? GL_RGBA : GL_RGB;
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);
        printf("Textura carregada: %s\n", path.c_str());
    } else {
        printf("Erro ao carregar textura: %s\n", path.c_str());
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    return texID;
}

void AnimatedModel::loadMaterialTextures() {
    materialTextures.resize(modelScene->mNumMaterials, 0);
    for (unsigned int i = 0; i < modelScene->mNumMaterials; i++) {
        aiMaterial* mat = modelScene->mMaterials[i];
        aiString texPath;
        if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
            std::string fullPath(texPath.C_Str());
            std::string fileName = fullPath.substr(fullPath.find_last_of("/\\") + 1);
            std::string localPath = modelDirectory + "/../Yoshimitsu_textures/" + fileName;
            materialTextures[i] = loadTexture(localPath);
        }
    }
}

void AnimatedModel::draw() {
    glBindVertexArray(VAO);
    for (size_t i = 0; i < meshEntries.size(); i++) {
        unsigned int matIdx = meshEntries[i].materialIndex;
        if (matIdx < materialTextures.size() && materialTextures[matIdx] != 0) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, materialTextures[matIdx]);
        }
        glDrawElements(GL_TRIANGLES,
                       (GLsizei)meshEntries[i].indexCount,
                       GL_UNSIGNED_INT,
                       (void*)(meshEntries[i].indexOffset * sizeof(unsigned int)));
    }
    glBindVertexArray(0);
}

float AnimatedModel::getAnimationDuration(const std::string& name) const {
    auto it = animations.find(name);
    if (it == animations.end()) return 0.0f;
    const Animation& a = it->second;
    if (a.ticksPerSecond == 0.0f) return 0.0f;
    return a.duration / a.ticksPerSecond; // duration in seconds
}