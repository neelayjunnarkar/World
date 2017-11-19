#include "TestModel.hpp"

const std::vector<Vertex> TestModel::vertex_data = {
        Vertex{glm::vec3(-1.f, -1.f, 0.0f),glm::vec3(0.f, 0.f, 1.f), glm::vec3(0.f, 178.f, 92.f)},
        Vertex{glm::vec3(0.f, -1.f, 0.0f), glm::vec3(0.f, 0.f, 1.f),glm::vec3(201.f, 0.f, 122.f)},        
        Vertex{glm::vec3(1.f, -1.f, 0.0f), glm::vec3(0.f, 0.f, 1.f),glm::vec3(255.f, 167.f, 0.f)},
        Vertex{glm::vec3(-1.f, 0.f, 0.0f), glm::vec3(0.f, 0.f, 1.f),glm::vec3(27.f,27.f,179.f)},
        Vertex{glm::vec3(0.f, 0.f, 0.0f), glm::vec3(0.f, 0.f, 1.f),glm::vec3(255.f, 116.f, 0.f)},
        Vertex{glm::vec3(1.f, 0.f, 0.0f), glm::vec3(0.f, 0.f, 1.f),glm::vec3(255.f, 255.f, 0.f)},
        Vertex{glm::vec3(-1.f, 1.f, 0.0f), glm::vec3(0.f, 0.f, 1.f),glm::vec3(255.f, 7.f, 0.f)},
        Vertex{glm::vec3(0.f, 1.f, 0.0f), glm::vec3(0.f, 0.f, 1.f),glm::vec3(11.f, 95.f, 165.f)},
        Vertex{glm::vec3(1.f, 1.f, 0.0f), glm::vec3(0.f, 0.f, 1.f),glm::vec3(98.f, 226.f, 0.f)}
};

const std::vector<GLushort> TestModel::element_data = {
        0,1,3, 3,1,4, 1,2,4, 4,2,5,
        3,4,6, 6,4,7, 4,5,7, 7,5,8
};

TestModel::TestModel()
    : Model(Mesh(vertex_data, element_data, GL_STATIC_DRAW))
{
    
}

void TestModel::update() {
    angle += glm::radians(.02f);
    rotation_axis.x = glm::sin(angle);
}