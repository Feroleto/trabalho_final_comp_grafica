#version 330 core

in vec2 texcoords_frag;
in vec3 normal_frag;

uniform sampler2D textureAlbedo;

out vec4 color;

void main()
{
    vec3 lightDir = normalize(vec3(1.0, 1.0, 0.5));
    float lambert = max(0.0, dot(normalize(normal_frag), lightDir));
    vec3 Kd = texture(textureAlbedo, texcoords_frag).rgb;
    color = vec4(Kd * (0.3 + lambert * 0.8), 1.0);
}