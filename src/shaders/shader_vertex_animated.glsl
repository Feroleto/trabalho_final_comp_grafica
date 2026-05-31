#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 boneWeights;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 boneMatrices[100];

out vec2 texcoords_frag;
out vec3 normal_frag;
out vec3 position_world_frag;

void main()
{
    // aplica transformacoes dos bones
    mat4 boneTransform  = boneMatrices[boneIDs[0]] * boneWeights[0];
         boneTransform += boneMatrices[boneIDs[1]] * boneWeights[1];
         boneTransform += boneMatrices[boneIDs[2]] * boneWeights[2];
         boneTransform += boneMatrices[boneIDs[3]] * boneWeights[3];

    vec4 skinnedPos    = boneTransform * vec4(position, 1.0);
    vec4 skinnedNormal = boneTransform * vec4(normal, 0.0);

    vec4 worldPos = model * skinnedPos;

    gl_Position = projection * view * worldPos;

    position_world_frag = worldPos.xyz;
    texcoords_frag = texCoords;
    normal_frag    = normalize(mat3(model) * vec3(skinnedNormal));
}