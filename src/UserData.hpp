#ifndef USERDATA_HPP
#define USERDATA_HPP

#include <GLES3/gl32.h>
#include <memory>
#include <vector>
#include "glm/glm.hpp"

class Model;

struct UserData {
    GLuint program_id           = NULL;
    glm::vec3 camera_pos        = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 camera_target     = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 up_vec            = glm::vec3(0.f, 1.f, 0.f); // should really have some sorta cache ting for view and perspective matrix
    
    glm::mat4 view_mat          = glm::mat4(); // identity matrix
    glm::mat4 projection_mat    = glm::mat4(); // identity matrix
    std::vector<std::unique_ptr<Model>> models; 

    struct Attributes {
        GLint position = NULL;
        GLint normal   = NULL;
        GLint diffuse_color    = NULL;
    } attributes;

    struct Uniforms {
        GLint model_mat      = NULL;
        GLint view_mat       = NULL;
        GLint projection_mat = NULL;
        GLint light_color    = NULL;
    } uniforms;
};


#endif