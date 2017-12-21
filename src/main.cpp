#include <GLES3/gl32.h>
extern "C" {
#include "esUtil.h"
}
#include <emscripten.h>
#include <string>
#include <fstream>
#include <algorithm>

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/transform.hpp"

#include "logging.hpp"

#include "SphericalFunctionModel.hpp"
#include "FunctionModel.hpp"
#include "TestModel.hpp"
#include <cmath>
#include "FastNoise.h"
#include <random>

bool init(ESContext *es_context);
bool initGL(ESContext *es_context);
void loop(void *data);
void update(ESContext *es_context);
void render(ESContext *es_context);
GLuint make_buffer(GLenum target, const void *buffer_data, GLsizei buffer_size, GLenum usage);


int main() {
    ESContext es_context;
    es_context.userData = new UserData;

    if (!init(&es_context)) {
        log("failed to initialize");
        return 1;
    }
    UserData *userdata = (UserData*)es_context.userData;

    auto basic_fn = [](float x, float y) -> float { return 0.f; };
    auto oscillations = [](float x, float y) -> float { return std::sin(x)+std::sin(y); };
    auto paraboloid = [](float x, float y) -> float { return -x*x - y*y; };
    auto spherical_sphere = [](float theta, float phi) -> float { return 5.f; };

    auto noisy = [](float theta, float phi) -> float { 
        static FastNoise gen;
        static float max = -10.f;
        static float min = 10.f;
        gen.SetNoiseType(FastNoise::PerlinFractal);
        gen.SetFrequency(M_PI);
        float val = gen.GetNoise(theta, phi);
        if (val > max) {
            max = val;
            log(std::string("max: ") + std::to_string(val));
        }
        if (val < min) {
            min = val;
            log(std::string("min: ") + std::to_string(val));
        }
        return 4.f + (val+0.611f)/(2.f*0.611f);
    };
    auto random = [](float x, float y) -> float {
        static std::default_random_engine gen;
        static std::uniform_real_distribution<float> dist(0.f, 1.f);
        return 4.f + dist(gen);
    };
    // userdata->models.push_back(std::make_unique<FunctionModel>(basic_fn, -1.f, 1.f, 3, -1.f, 1.f, 3));
    // userdata->models.push_back(std::make_unique<FunctionModel>(oscillations, -2.f*M_PI, 2.f*M_PI, 100,
    //    -2.f*M_PI, 2.f*M_PI, 100));
    
    // userdata->models.push_back(std::make_unique<SphericalFunctionModel>(spherical_sphere, 
    //     0.f, 2.f*M_PI, 75, 
    //     0.f, (float)M_PI, 75, true));
    userdata->models.push_back(std::make_unique<SphericalFunctionModel>(noisy, 0.f, 2.f*M_PI, 100, 0.f, (float)M_PI, 100, true));

    userdata->camera_pos = glm::vec3(0,-30, 0);
    userdata->camera_target = glm::vec3(0,0,0);
    userdata->up_vec = glm::vec3(0,0,-1);
        
    emscripten_set_main_loop_arg(&loop, (void*)&es_context, 0, 1);

    delete (UserData*)es_context.userData;
}

bool init(ESContext *es_context) {

    int height = EM_ASM_INT({
        return window.innerHeight;
    });
    int width = EM_ASM_INT({
        return window.innerWidth;
    });

    esInitContext(es_context);
    esCreateWindow(es_context, "rawr", width, height, ES_WINDOW_RGB);

    UserData *userdata = (UserData*)es_context->userData;

    if (!initGL(es_context)) {
        log("Unable to initialize OpenGL\n\r");
        return false;
    }

    float field_of_view = 45.0f;
    userdata->projection_mat = glm::perspective(
        glm::radians(field_of_view),
        ((float)(es_context->width))/((float)es_context->height),
        0.1f,
        100.0f
    );

    userdata->view_mat = glm::lookAt(
        userdata->camera_pos,
        userdata->camera_target,
        userdata->up_vec
    );

    return true;
}

bool initGL(ESContext *es_context) {
    UserData *userdata = (UserData*)es_context->userData;

    std::string vertexshader_src; 
    const std::string vertexshader_file = "shaders/vertex_shader.glessl";

    std::string fragmentshader_src;
    const std::string fragmentshader_file = "shaders/fragment_shader.glessl";

    std::ifstream vertexshader_stream(vertexshader_file);
    if (vertexshader_stream.is_open()) {
        std::string line = "";
        if (std::getline(vertexshader_stream, line)) {
            vertexshader_src += line;
            while (std::getline(vertexshader_stream, line))
                vertexshader_src += "\n" + line;
        }
        vertexshader_stream.close();
    } else {
        log("could not open " + vertexshader_file);
        return false;
    }

    std::ifstream fragmentshader_stream(fragmentshader_file);
    if (fragmentshader_stream.is_open()) {
        std::string line = "";
        if (std::getline(fragmentshader_stream, line)) {
            fragmentshader_src += line;
            while (std::getline(fragmentshader_stream, line))
                fragmentshader_src += "\n" + line;
        }
        fragmentshader_stream.close();
    } else {
        log("could not open " + fragmentshader_file);
        return false;
    }

    userdata->program_id = esLoadProgram(vertexshader_src.c_str(), fragmentshader_src.c_str());

    if (userdata->program_id == 0)
        return false;

    userdata->uniforms.model_mat =  glGetUniformLocation(userdata->program_id, "model_mat");
    userdata->uniforms.view_mat = glGetUniformLocation(userdata->program_id, "view_mat");
    userdata->uniforms.projection_mat = glGetUniformLocation(userdata->program_id, "projection_mat");
    
    userdata->attributes.position = glGetAttribLocation(userdata->program_id, "position_modelspace");
    userdata->attributes.normal   = glGetAttribLocation(userdata->program_id, "normal_modelspace");
    userdata->attributes.diffuse_color = glGetAttribLocation(userdata->program_id, "diffuse_color");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_CULL_FACE);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    return true;
}

void loop(void *data) {
    update((ESContext*)data);
    render((ESContext*)data);
}

void update(ESContext *es_context) {
    UserData *userdata = (UserData*)es_context->userData;

    for (std::unique_ptr<Model> &model : userdata->models) {
        model->update();
    }
    userdata->models[0]->angle += 0.0005;

    userdata->view_mat = glm::lookAt(
        userdata->camera_pos,
        userdata->camera_target,
        userdata->up_vec
    );

}

void render(ESContext *es_context) {
    UserData *userdata = (UserData*)es_context->userData;

    glUseProgram(userdata->program_id);

    // set view and projection matrices
    glUniformMatrix4fv(userdata->uniforms.view_mat, 1, GL_FALSE, &userdata->view_mat[0][0]);    
    glUniformMatrix4fv(userdata->uniforms.projection_mat, 1, GL_FALSE, &userdata->projection_mat[0][0]);
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnableVertexAttribArray(userdata->attributes.position);
    glEnableVertexAttribArray(userdata->attributes.normal);
    glEnableVertexAttribArray(userdata->attributes.diffuse_color);

    for (auto &model : userdata->models) {
        model->render(userdata);
    }

    glDisableVertexAttribArray(userdata->attributes.diffuse_color);
    glDisableVertexAttribArray(userdata->attributes.normal);    
    glDisableVertexAttribArray(userdata->attributes.position);

    eglSwapBuffers(es_context->eglDisplay, es_context->eglSurface);
}