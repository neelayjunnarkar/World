#ifndef MODEL_HPP
#define MODEL_HPP

#include "Mesh.hpp"

class Model {
protected:
    Mesh mesh;
    glm::vec3 scale                 = glm::vec3(1.f, 1.f, 1.f);
    glm::vec3 rotation_axis         = glm::vec3(0.f, 0.f, 1.f);
    float angle                     = 0.f; // radians
    glm::vec3 translation           = glm::vec3(0.f, 0.f, 0.f);
public:
    Model();
    
    Model(Mesh mesh);

    virtual void update() = 0;

    void render(const UserData *data);
};

#endif