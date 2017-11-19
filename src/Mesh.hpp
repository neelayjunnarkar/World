#ifndef MESH_HPP
#define MESH_HPP

#include <GLES3/gl32.h>
#include <vector>
#include <memory>
#include "glm/glm.hpp"

#include "UserData.hpp"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 diffuse_color;
};

class Mesh {
private:
    GLuint vertex_buffer            = NULL;
    GLuint normal_buffer            = NULL;
    GLuint element_buffer           = NULL;
    GLuint diffuse_color_buffer     = NULL;
    GLsizei element_count           = NULL;
    std::vector<Vertex> vertex_data;
public:

    Mesh();

    Mesh(std::vector<Vertex> vertex_data, std::vector<GLushort> element_data, GLenum hint);

    void render(const UserData *data);
};

#endif