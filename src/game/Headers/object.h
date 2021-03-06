#ifndef OBJECT_H
#define OBJECT_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <tiny_obj_loader.h>
#include <vector>
#include <map>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

#include "material.h"

enum OBJ_TYPE {
    PLAYER=0,
    NPC_OBJ,
    PLAYER_TARGET,
    STATIC
};

enum POSSIBLE_MOV {
    FRONT=0,
    BACK,
    RIGHT,
    LEFT,
    SPEED_UP,
    SPEED_DOWN,
    JUMP
};

// Estrutura para carregar o modelo
struct GameModel
{
    GameModel(const char* filename, const char* go_name = NULL, const char* basepath = NULL, bool triangulate = true);
    virtual ~GameModel();
    tinyobj::attrib_t                 attrib;
    std::vector<tinyobj::shape_t>     shapes;
    std::vector<tinyobj::material_t>  materials;
    glm::vec3 bbox_min;
    glm::vec3 bbox_max;
    std::string go_name;
};

// Estrutura para, dado um modelo, criar um objeto na cena
struct GameObject {
    GameObject(std::string name, GameModel& model, glm::vec4 position, glm::vec3 scale, glm::vec3 rotation=glm::vec3(), GameObject* father=nullptr);
    std::string name; // Nome do objeto (não foi utilizado)
    GameModel& model; // Referência ao modelo que será usado
    glm::vec4 position; // Valores da matriz de Translação
    glm::vec3 scale; // Valores da matriz de Escala
    glm::vec3 rotation; // Valores da matriz de Rotação
    glm::vec4 move_direction; // Vetor que aponta o sentido de movimento
    int type; // Tipo para o shader
    int object_type; // Tipo de objeto para o loop principal
    GameObject* father; // Pai do objeto para herança (não foi utilizado)
    Material material; // Material para cor sólida (padrão cinza)

    void setMaterial(Material material);
};

// Player: subclasse de GameObject, pode ser controlada pelo usuário
struct Player : public GameObject {
    Player(GameObject& object, bool can_move, float speed);
    bool can_move;
    float current_speed; 
    float base_speed; // Velocidade inicial para caso a velocidade mude (não foi utilizado)
    float move_angle;

    void updateMovement(std::map<POSSIBLE_MOV, bool*> pressed_keys, const float delta_t);
};

struct PlayerTarget : public GameObject {
    PlayerTarget(GameObject& object, bool was_caught);
    bool was_caught;
};

struct NPC : public GameObject {
    NPC(GameObject& object, float t_position, float t_speed, glm::vec4 p0, glm::vec4 p1, glm::vec4 p2, glm::vec4 p3);
    float t_position; // Posição do objeto na curva de bezier
    float t_speed; // "curvas por segundo" (volta completa pela curva de bezier)
    glm::vec4 p0; // ponto 0 da curva de bezier
    glm::vec4 p1; // ponto 1 da curva de bezier
    glm::vec4 p2; // ponto 2 da curva de bezier
    glm::vec4 p3; // ponto 3 da curva de bezier

    void updateMovement(const float delta_t);
};

#endif // OBJECT_H
