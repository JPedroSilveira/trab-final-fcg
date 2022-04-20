#include "../Headers/object.h"

GameModel::GameModel(const char* filename, const char* object_name, const char* basepath, bool triangulate)
{
    printf("Carregando modelo \"%s\"... ", filename);

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename, basepath, triangulate);

    if (!err.empty())
        fprintf(stderr, "\n%s\n", err.c_str());

    if (!ret)
        throw std::runtime_error("Erro ao carregar modelo.");

    printf("OK.\n");

    this->go_name = std::string(object_name);
}

GameModel::~GameModel()
{
    //dtor
}


GameObject::GameObject(std::string name, GameModel& model, glm::vec4 position, glm::vec3 scale, glm::vec3 rotation, GameObject* father)
 : name(name), model(model), position(position), scale(scale), rotation(rotation), move_direction(glm::vec4(1,0,0,0)),
  father(father), material(base_material) {}


void GameObject::setMaterial(Material material)
{
    this->material.Kd = material.Kd;
    this->material.Ks = material.Ks;
    this->material.Ka = material.Ka;
    this->material.q = material.q;
}


Player::Player(GameObject& object, bool can_move, float speed) :
GameObject(object), can_move(can_move), current_speed(speed), base_speed(speed) {}

void Player::updateMovement(std::map<POSSIBLE_MOV, bool*> pressed_keys, const float delta_t)
{
    if (can_move)
    {
        if (*pressed_keys[FRONT])
        {
            glm::vec4 direction(rotation.x, rotation.y, rotation.z, 0);
            this->position += direction * current_speed * delta_t;
        }
        if (*pressed_keys[BACK])
        {
            glm::vec4 direction(rotation.x, rotation.y, rotation.z, 0);
            this->position -= direction * current_speed * delta_t;
        }
        if (*pressed_keys[RIGHT])
        {
            this->rotation.y += current_speed * delta_t;
        }
        if (*pressed_keys[LEFT])
        {
            this->rotation.z -= current_speed * delta_t;
        }
    }
}