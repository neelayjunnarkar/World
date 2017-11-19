#include "Model.hpp"


#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

Model::Model() {}

Model::Model(Mesh mesh):
    mesh{mesh}
{
}

void Model::render(const UserData *data) {
    // compute model matrix
    glm::mat4 model_mat = glm::translate(glm::mat4(), translation)
        * glm::rotate(glm::degrees(angle), rotation_axis)
        * glm::scale(scale);

    // set model matrix
    glUniformMatrix4fv(data->uniforms.model_mat, 1, GL_FALSE, &model_mat[0][0]);

    mesh.render(data);

}