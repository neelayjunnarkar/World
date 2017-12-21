#include "FunctionModel.hpp"
#include <cmath>
#include <string>
#include <vector>
#include "logging.hpp"
#include "glm/glm.hpp"

FunctionModel::FunctionModel(std::function<float(float, float)> fn,
    const float x_min, const float x_max, const int x_samples,
    const float y_min, const float y_max, const int y_samples):
    f(fn) {

    const float x_step = (x_max-x_min)/(x_samples-1);
    const float y_step = (y_max-y_min)/(y_samples-1);

    // converts coordinates in 2d to flattened coordinates
    auto r2tor = [x_samples](const int x, const int y) -> int { return x + y * x_samples; };
    
    std::vector<Vertex> vertex_data(x_samples*y_samples);
    std::vector<GLushort> element_data;

    // Set up vertex_data
    int max_x_int = -1;
    int max_y_int = -1;

    {
        int x_int = 0;
        for (float x_real = x_min; x_real < (2.f*x_max + x_step)/2.f; x_real += x_step, ++x_int) {
            max_x_int = x_int;
            
            int y_int = 0;
            for (float y_real = y_min; y_real < (2.f*y_max + y_step)/2.f; y_real += y_step, ++y_int) {
                max_y_int = y_int;
                vertex_data[r2tor(x_int, y_int)] = Vertex{glm::vec3(x_real, y_real, f(x_real, y_real)), glm::vec3(0.f, 0.f, 0.f), glm::vec3(66.f, 134.f, 244.f)};
            }
        }
    }

    // Set up element data

    for (int x_int = 0; x_int <= max_x_int - 1; ++x_int) {
        for (int y_int = 0; y_int <= max_y_int - 1; ++y_int) {
            const int bottom_left = r2tor(x_int, y_int);
            const int bottom_right = r2tor(x_int+1, y_int);
            const int top_left = r2tor(x_int, y_int+1);
            const int top_right = r2tor(x_int+1, y_int+1);

            element_data.push_back(top_left);
            element_data.push_back(bottom_left);
            element_data.push_back(top_right);

            element_data.push_back(bottom_left);
            element_data.push_back(bottom_right);
            element_data.push_back(top_right);

            glm::vec3 a1 = vertex_data[top_left].position - vertex_data[bottom_left].position ; 
            glm::vec3 a2 = vertex_data[top_right].position - vertex_data[bottom_left].position ; 
            glm::vec3 a_normal = glm::cross(a2, a1);

            glm::vec3 b1 = vertex_data[top_right].position - vertex_data[bottom_left].position ; 
            glm::vec3 b2 = vertex_data[bottom_right].position - vertex_data[bottom_left].position ; 
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
void FunctionModel::update() {

}