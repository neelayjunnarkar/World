#include "Mesh.hpp"

Mesh::Mesh() {}

Mesh::Mesh(std::vector<Vertex> vertex_data, std::vector<GLushort> element_data, GLenum hint):
    element_count{(GLsizei)element_data.size()}
{   
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(
        GL_ARRAY_BUFFER, 
        vertex_data.size()*sizeof(Vertex),
        vertex_data.data(),
        hint
    );

    glGenBuffers(1, &normal_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertex_data.size()*sizeof(Vertex),
        vertex_data.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &diffuse_color_buffer);    
    glBindBuffer(GL_ARRAY_BUFFER, diffuse_color_buffer);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertex_data.size()*sizeof(Vertex),
        vertex_data.data(),
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &element_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        element_count * sizeof(GLushort),
        element_data.data(),
        GL_STATIC_DRAW
    );
}

void Mesh::render(const UserData *data) {
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(
        data->attributes.position,
        3, GL_FLOAT, GL_FALSE, 
        sizeof(Vertex),
        (void*)offsetof(Vertex, position)
    );

    glBindBuffer(GL_ARRAY_BUFFER, normal_buffer);
    glVertexAttribPointer(
        data->attributes.normal,
        3, GL_FLOAT, GL_FALSE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, normal)
    );

    glBindBuffer(GL_ARRAY_BUFFER, diffuse_color_buffer);
    glVertexAttribPointer(
        data->attributes.diffuse_color,
        3, GL_FLOAT, GL_TRUE,
        sizeof(Vertex),
        (void*)offsetof(Vertex, diffuse_color)
    );

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glDrawElements(
        GL_TRIANGLES,
        element_count,
        GL_UNSIGNED_SHORT,
        (void*)0
    );

}