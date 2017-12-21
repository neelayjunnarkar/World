#include "SphericalFunctionModel.hpp"
#include "logging.hpp"
#include <string>

SphericalFunctionModel::SphericalFunctionModel(std::function<float(float, float)> f,
    const float theta_min, const float theta_max, const int theta_samples,
    const float phi_min, const float phi_max, const int phi_samples,
    const bool stitch) {

    const float theta_step = (theta_max-theta_min)/(theta_samples-1);
    const float phi_step = (phi_max-phi_min)/(phi_samples-1);

    // converts coordinates in 2d to flattened coordinates
    auto r2tor = [theta_samples](const int theta, const int phi) -> int { return theta + phi * theta_samples; };
    
    std::vector<Vertex> vertex_data(theta_samples*phi_samples);
    std::vector<GLushort> element_data;

    // Set up vertex_data
    int max_theta_int = -1;
    int max_phi_int = -1;

    {
        int theta_int = 0;
        for (float theta_real = theta_min; theta_real < (2.f*theta_max + theta_step)/2.; theta_real += theta_step, ++theta_int) {
            max_theta_int = theta_int;
            
            int phi_int = 0;
            for (float phi_real = phi_min; phi_real < (2.f*phi_max + phi_step)/2.f; phi_real += phi_step, ++phi_int) {
                max_phi_int = phi_int;
                float r = f(theta_real, phi_real);
                float sin_phi = glm::sin(phi_real);

                vertex_data[r2tor(theta_int, phi_int)] = Vertex{
                    glm::vec3(r*sin_phi*glm::cos(theta_real), r*sin_phi*glm::sin(theta_real), r*glm::cos(phi_real)), 
                    glm::vec3(0.f, 0.f, 0.f), 
                    glm::vec3(66.f, 134.f, 244.f)};
            }
        }
    }

    // Set up element data

    for (int theta_int = 0; theta_int <= max_theta_int - 1; ++theta_int) {
        for (int phi_int = 0; phi_int <= max_phi_int - 1; ++phi_int) {
            const int bottom_left = r2tor(theta_int, phi_int);
            const int bottom_right = r2tor(theta_int+1, phi_int);
            const int top_left = r2tor(theta_int, phi_int+1);
            const int top_right = r2tor(theta_int+1, phi_int+1);

            element_data.push_back(top_left);
            element_data.push_back(bottom_left);
            element_data.push_back(top_right);

            element_data.push_back(bottom_left);
            element_data.push_back(bottom_right);
            element_data.push_back(top_right);

            glm::vec3 a1 = vertex_data[top_left].position - vertex_data[bottom_left].position; 
            glm::vec3 a2 = vertex_data[top_right].position - vertex_data[bottom_left].position; 
            glm::vec3 a_normal = glm::cross(a2, a1);

            glm::vec3 b1 = vertex_data[top_right].position - vertex_data[bottom_left].position; 
            glm::vec3 b2 = vertex_data[bottom_right].position - vertex_data[bottom_left].position; 
            glm::vec3 b_normal = glm::cross(b2, b1);

            vertex_data[bottom_left].normal += a_normal + b_normal;
            vertex_data[top_left].normal += a_normal;
            vertex_data[top_right].normal += a_normal + b_normal;
            vertex_data[bottom_right].normal += b_normal;
        }
    }

    if (stitch) {
        // assumes f(max theta, phi) = f(min theta, phi)

        // link max theta to min theta + theta step
        for (int phi_int = 0; phi_int <= max_phi_int - 1; ++phi_int) {
            const int bottom_left = r2tor(max_theta_int, phi_int);
            const int bottom_right = r2tor(1, phi_int);
            const int top_left = r2tor(max_theta_int, phi_int+1);
            const int top_right = r2tor(1, phi_int+1);

            glm::vec3 a1 = vertex_data[top_left].position - vertex_data[bottom_left].position; 
            glm::vec3 a2 = vertex_data[top_right].position - vertex_data[bottom_left].position; 
            glm::vec3 a_normal = glm::cross(a2, a1);

            glm::vec3 b1 = vertex_data[top_right].position - vertex_data[bottom_left].position; 
            glm::vec3 b2 = vertex_data[bottom_right].position - vertex_data[bottom_left].position; 
            glm::vec3 b_normal = glm::cross(b2, b1);

            vertex_data[bottom_left].normal += a_normal + b_normal;
            vertex_data[top_left].normal += a_normal;
            vertex_data[top_right].normal += a_normal + b_normal;
            vertex_data[bottom_right].normal += b_normal;
        }

        // link max theta - theta step to min theta
        for (int phi_int = 0; phi_int <= max_phi_int - 1; ++phi_int) {
            const int bottom_left = r2tor(max_theta_int-1, phi_int);
            const int bottom_right = r2tor(0, phi_int);
            const int top_left = r2tor(max_theta_int-1, phi_int+1);
            const int top_right = r2tor(0, phi_int+1);

            glm::vec3 a1 = vertex_data[top_left].position - vertex_data[bottom_left].position; 
            glm::vec3 a2 = vertex_data[top_right].position - vertex_data[bottom_left].position; 
            glm::vec3 a_normal = glm::cross(a2, a1);

            glm::vec3 b1 = vertex_data[top_right].position - vertex_data[bottom_left].position; 
            glm::vec3 b2 = vertex_data[bottom_right].position - vertex_data[bottom_left].position; 
            glm::vec3 b_normal = glm::cross(b2, b1);

            vertex_data[bottom_left].normal += a_normal + b_normal;
            vertex_data[top_left].normal += a_normal;
            vertex_data[top_right].normal += a_normal + b_normal;
            vertex_data[bottom_right].normal += b_normal;
        }
    }

    for (Vertex &vertex : vertex_data) {
        vertex.normal = glm::normalize(vertex.normal);
    }

    mesh = Mesh(vertex_data, element_data, GL_STATIC_DRAW);
}

void SphericalFunctionModel::update() {

}