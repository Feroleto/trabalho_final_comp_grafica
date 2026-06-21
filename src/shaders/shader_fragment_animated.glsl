#version 330 core

in vec2 texcoords_frag;
in vec3 normal_frag;
in vec3 position_world_frag;

uniform vec3 player_sword_light_pos;
uniform vec3 enemy_sword_light_pos;
uniform bool PlanarShadow;

uniform sampler2D textureAlbedo;

out vec4 color;

void main()
{
    if (PlanarShadow) {
        color = vec4(0.05, 0.1, 0.0, 0.9);
        return;
    }

    vec3 lightDir = normalize(vec3(1.0, 1.0, 0.5));
    float lambert = max(0.0, dot(normalize(normal_frag), lightDir));

    //Luz da espada do player
    vec3 sword_light_vec = player_sword_light_pos - position_world_frag;
    vec3 sword_l = normalize(sword_light_vec);
    vec3 sword_color = vec3(0.0, 0.7, 0.3);

    float sword_lambert = max(0.0, dot(normalize(normal_frag), sword_l));
    float sword_distance = length(sword_light_vec);

    float sword_attenuation =
        1.0 / (1.0 + 0.09 * sword_distance +
                      0.032 * sword_distance * sword_distance);

    //Luz da espada do inimigo
    vec3 enemy_sword_light_vec = enemy_sword_light_pos - position_world_frag;
    vec3 enemy_sword_l = normalize(enemy_sword_light_vec);
    vec3 enemy_sword_color = vec3(0.0, 0.8, 0.0);

    float enemy_sword_lambert =
        max(0.0, dot(normalize(normal_frag), enemy_sword_l));

    float enemy_sword_distance = length(enemy_sword_light_vec);

    float enemy_sword_attenuation =
        1.0 / (1.0 + 0.09 * enemy_sword_distance +
                      0.032 * enemy_sword_distance * enemy_sword_distance);

    float sword_intensity = 1.0;

    // texturas carregadas como GL_SRGB8 — não aplicar gamma aqui
    vec3 Kd = texture(textureAlbedo, texcoords_frag).rgb;

    vec3 sword_light_p =
        Kd * sword_color *
        sword_lambert *
        sword_attenuation *
        sword_intensity;

    vec3 sword_light_e =
        Kd * enemy_sword_color *
        enemy_sword_lambert *
        enemy_sword_attenuation *
        sword_intensity;

    vec3 baseLight = Kd * (0.3 + lambert * 0.8);

    color = vec4(baseLight + sword_light_p + sword_light_e, 1.0);
}
