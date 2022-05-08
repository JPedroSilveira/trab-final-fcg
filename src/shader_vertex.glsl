#version 330 core

// Atributos de v�rtice recebidos como entrada ("in") pelo Vertex Shader.
// Veja a fun��o BuildTrianglesAndAddToVirtualScene() em "main.cpp".
layout (location = 0) in vec4 model_coefficients;
layout (location = 1) in vec4 normal_coefficients;
layout (location = 2) in vec2 texture_coefficients;

// Matrizes computadas no c�digo C++ e enviadas para a GPU
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Atributos de vértice que serão gerados como saída ("out") pelo Vertex Shader.
// ** Estes serão interpolados pelo rasterizador! ** gerando, assim, valores
// para cada fragmento, os quais serão recebidos como entrada pelo Fragment
// Shader. Veja o arquivo "shader_fragment.glsl".
out vec4 position_world;
out vec4 position_model;
out vec4 normal;
out vec2 texcoords;
out vec4 color_v;

// Veja o fragment_shader.glsl
struct Material {
    vec3 Kd;
    vec3 Ks;
    vec3 Ka;
    float q;
};

uniform int object_id;
uniform Material material;

#define MATERIAL_GOURAUD 1 // Calcula apenas o material

void main()
{
    // A variável gl_Position define a posição final de cada vértice
    // OBRIGATORIAMENTE em "normalized device coordinates" (NDC), onde cada
    // coeficiente estará entre -1 e 1 após divisão por w.
    // Veja {+NDC2+}.
    //
    // O código em "main.cpp" define os vértices dos modelos em coordenadas
    // locais de cada modelo (array model_coefficients). Abaixo, utilizamos
    // operações de modelagem, definição da câmera, e projeção, para computar
    // as coordenadas finais em NDC (variável gl_Position). Após a execução
    // deste Vertex Shader, a placa de vídeo (GPU) fará a divisão por W. Veja
    // slides 41-67 e 69-86 do documento Aula_09_Projecoes.pdf.

    gl_Position = projection * view * model * model_coefficients;

    // Posição do vértice atual no sistema de coordenadas global (World).
    position_world = model * model_coefficients;
    // Posição do vértice atual no sistema de coordenadas local do modelo.
    position_model = model_coefficients;
    // Normal do vértice atual no sistema de coordenadas global (World).
    // Veja slides 123-151 do documento Aula_07_Transformacoes_Geometricas_3D.pdf.
    normal = inverse(transpose(model)) * normal_coefficients;
    normal.w = 0.0;

    // Coordenadas de textura obtidas do arquivo OBJ (se existirem!)
    texcoords = texture_coefficients;
    
    // Define a cor de acordo com os vértices
    if (object_id == MATERIAL_GOURAUD)
    {
        // Variáve
        vec4 l = normalize(vec4(1.0,1.0,0.5,0.0));
        vec4 n = normalize(normal);
        float lambert = max(0, dot(n,l));
        vec3 I = vec3(1.0,1.0,1.0);
        // Espectro da luz ambiente
        vec3 Ia = vec3(0.2, 0.2, 0.2);
        // Termo difuso utilizando a lei dos cossenos de Lambert
        vec3 lambert_diffuse_term = material.Kd * I * max(0, dot(n, l));
        // Termo ambiente
        vec3 ambient_term = material.Ka * Ia;
        // Termo especular utilizando o modelo de iluminação de Phong
        // vec3 phong_specular_term  = material.Ks * I * pow(max(0, dot(h, n)), material.q);
        // Alpha
        color_v.a = 1;
        // Cor final do fragmento calculada com uma combinação dos termos difuso,
        // especular, e ambiente. Veja slide 129 do documento Aula_17_e_18_Modelos_de_Iluminacao.pdf.
        color_v.rgb = lambert_diffuse_term + ambient_term;
    } else {
        color_v = vec4(0,0,0,1);
    }
}