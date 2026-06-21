#version 330 core

// Atributos de fragmentos recebidos como entrada ("in") pelo Fragment Shader.
// Neste exemplo, este atributo foi gerado pelo rasterizador como a
// interpolação da posição global e a normal de cada vértice, definidas em
// "shader_vertex.glsl" e "main.cpp".
in vec4 position_world;
in vec4 normal;

// Posição do vértice atual no sistema de coordenadas local do modelo.
in vec4 position_model;

// Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
in vec2 texcoords;

// Matrizes computadas no código C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Posicao da luz da espada
uniform vec4 player_sword_light_pos;
uniform vec4 enemy_sword_light_pos;

// Identificador que define qual objeto está sendo desenhado no momento
#define PLANE  0
#define BACKGROUND 1
#define SWORD 2
#define PROJECTILE 3
#define DEBUG_BOX 4
#define DEBUG_BOX_SWORD 5
uniform int object_id;

// Parâmetros da axis-aligned bounding box (AABB) do modelo
uniform vec4 bbox_min;
uniform vec4 bbox_max;

//

// Variáveis para acesso das imagens de textura
uniform sampler2D TextureImage0; // PLANE
uniform sampler2D TextureImage1; // BACKGROUND
uniform sampler2D TextureImage2; // SWORD
uniform sampler2D TextureImage3; // PROJECTILE


uniform bool PlanarShadow;////para sombra da espada


// O valor de saída ("out") de um Fragment Shader é a cor final do fragmento.
out vec4 color;

// Constantes
#define M_PI   3.14159265358979323846
#define M_PI_2 1.57079632679489661923

void main()
{

    if (PlanarShadow) {
        color = vec4(0.05, 0.1, 0.0, 0.9);
        return;
    }/////Para a sombra da espada

    // Obtemos a posição da câmera utilizando a inversa da matriz que define o
    // sistema de coordenadas da câmera.
    vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 camera_position = inverse(view) * origin;

    // O fragmento atual é coberto por um ponto que percente à superfície de um
    // dos objetos virtuais da cena. Este ponto, p, possui uma posição no
    // sistema de coordenadas global (World coordinates). Esta posição é obtida
    // através da interpolação, feita pelo rasterizador, da posição de cada
    // vértice.
    vec4 p = position_world;

    // Normal do fragmento atual, interpolada pelo rasterizador a partir das
    // normais de cada vértice.
    vec4 n = normalize(normal);

    // Vetor que define o sentido da fonte de luz em relação ao ponto atual.
    vec4 l = normalize(vec4(1.0,1.0,0.0,0.0));

    // Calculos das iluminacoes das espadas
    //PLAYER
    vec4 player_sword_light_vec = player_sword_light_pos - p;
    vec4 player_sword_l = normalize(player_sword_light_vec);

    vec3 player_sword_color = vec3(0.0, 0.7, 0.3);

    float player_sword_lambert =
        max(0.0, dot(n, player_sword_l));

    float player_sword_distance =
        length(player_sword_light_vec.xyz);

    float player_sword_attenuation =
        1.0 / (1.0 +
            0.09 * player_sword_distance +
            0.032 * player_sword_distance * player_sword_distance);

    //ENEMY
    vec4 enemy_sword_light_vec = enemy_sword_light_pos - p;
    vec4 enemy_sword_l = normalize(enemy_sword_light_vec);

    vec3 enemy_sword_color = vec3(0.0, 0.8, 0.0);

    float enemy_sword_lambert =
        max(0.0, dot(n, enemy_sword_l));

    float enemy_sword_distance =
        length(enemy_sword_light_vec.xyz);

    float enemy_sword_attenuation =
        1.0 / (1.0 +
            0.09 * enemy_sword_distance +
            0.032 * enemy_sword_distance * enemy_sword_distance);

    float sword_intensity = 3.0;

    // Vetor que define o sentido da câmera em relação ao ponto atual.
    vec4 v = normalize(camera_position - p);

    // Coordenadas de textura U e V
    float U = 0.0;
    float V = 0.0;

	// Coeficiente de refletância difusa
	vec3 Kd0;


    if ( object_id == PLANE )
    {
        // Coordenadas de textura do plano, obtidas do arquivo OBJ.
        vec2 uv = position_world.xz * 0.15;


		// Obtemos a refletância difusa a partir da leitura da imagem TextureImage0
	        Kd0 = texture(TextureImage0, uv).rgb;
    }
    else if ( object_id == BACKGROUND )
    {
        float u = 0.5 + atan(position_model.z, position_model.x) / (2.0 * M_PI);
        float v = (position_model.y - bbox_min.y) / (bbox_max.y - bbox_min.y);
        float repeatU = 2.0; // number of horizontal repetitions
        vec2 uv = vec2(u * repeatU, v);

        Kd0 = texture(TextureImage1, uv).rgb;
    }
    else if ( object_id == SWORD)
    {
        U = texcoords.x;
        V = texcoords.y;
        Kd0 = texture(TextureImage2, vec2(U,V)).rgb;
    }
    else if ( object_id == PROJECTILE)
    {
        U = texcoords.x;
        V = texcoords.y;
        Kd0 = texture(TextureImage3, vec2(U,V)).rgb;
    }
    else if ( object_id == DEBUG_BOX )
    {
        Kd0 = vec3(1.0, 0.0, 0.0);
    }
    else if ( object_id == DEBUG_BOX_SWORD )
    {
        Kd0 = vec3(0.0, 0.0, 1.0);
    }
    else
    {
        Kd0 = vec3(1.0, 1.0, 1.0);
    }

    //luz da espada
    vec3 sword_light_p = Kd0 * player_sword_color *
                         player_sword_lambert *
                         player_sword_attenuation *
                         sword_intensity;

    vec3 sword_light_e = Kd0 * enemy_sword_color *
                         enemy_sword_lambert *
                         enemy_sword_attenuation *
                         sword_intensity;

    
    // Equação de Iluminação
    float lambert = max(0.0 ,dot(n,l));

    float ambient = 0.6;

    color.rgb = Kd0 * (ambient + lambert) + sword_light_p + sword_light_e;

    

    // NOTE: Se você quiser fazer o rendering de objetos transparentes, é
    // necessário:
    // 1) Habilitar a operação de "blending" de OpenGL logo antes de realizar o
    //    desenho dos objetos transparentes, com os comandos abaixo no código C++:
    //      glEnable(GL_BLEND);
    //      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // 2) Realizar o desenho de todos objetos transparentes *após* ter desenhado
    //    todos os objetos opacos; e
    // 3) Realizar o desenho de objetos transparentes ordenados de acordo com
    //    suas distâncias para a câmera (desenhando primeiro objetos
    //    transparentes que estão mais longe da câmera).
    // Alpha default = 1 = 100% opaco = 0% transparente
    color.a = 1;

    // Cor final com correção gamma, considerando monitor sRGB.
    // Veja https://en.wikipedia.org/w/index.php?title=Gamma_correction&oldid=751281772#Windows.2C_Mac.2C_sRGB_and_TV.2Fvideo_standard_gammas
    color.rgb = pow(color.rgb, vec3(1.0/2.2));
}
