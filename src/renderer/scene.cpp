#include "renderer/scene.hpp"
#include "renderer/shader.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <shaders/basic_frag.hpp>
#include <shaders/basic_vert.hpp>
#include <shaders/circle_frag.hpp>

Scene::Scene() {
    // build and compile our shader program
    // ------------------------------------
    // These throw on failure (see renderer/shader.hpp), so a broken shader
    // stops the app with the driver's message instead of leaving us with a
    // program that silently draws nothing.
    unsigned int vertex_shader =
        compile_shader(GL_VERTEX_SHADER, shaders::basic_vert, "basic.vert");
    unsigned int fragment_shader =
        compile_shader(GL_FRAGMENT_SHADER, shaders::circle_frag, "circle.frag");

    shader_program_ = link_program(vertex_shader, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // Uniform locations are fixed for the life of the linked program, so look
    // this up once here rather than every frame in render().
    transform_loc_ = glGetUniformLocation(shader_program_, "transform");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        0.5f,  0.5f,  0.0f, // top right
        1.0f,  1.0f,        // uv
        0.5f,  -0.5f, 0.0f, // bottom right
        1.0f,  0.0f,        // uv
        -0.5f, -0.5f, 0.0f, // bottom left
        0.0f,  0.0f,        // uv
        -0.5f, 0.5f,  0.0f, // top left
        0.0f,  1.0f         // uv
    };
    unsigned int indices[] = {
        // note that we start from 0!
        0,
        3,
        1, // first Triangle
        1,
        3,
        2 // second Triangle
    };
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s),
    // and then configure vertex attributes(s).
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW
    );

    glVertexAttribPointer(
        0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0
    );
    glEnableVertexAttribArray(0);
    // uv
    glVertexAttribPointer(
        1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))
    );
    glEnableVertexAttribArray(1);

    // note that this is allowed, the call to glVertexAttribPointer registered
    // VBO as the vertex attribute's bound vertex buffer object so afterwards we
    // can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // remember: do NOT unbind the EBO while a VAO is active as the bound
    // element buffer object IS stored in the VAO; keep the EBO bound.
    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally
    // modify this VAO, but this rarely happens. Modifying other VAOs requires a
    // call to glBindVertexArray anyways so we generally don't unbind VAOs (nor
    // VBOs) when it's not directly necessary.
    glBindVertexArray(0);
}

Scene::~Scene() {
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_);
    glDeleteBuffers(1, &ebo_);
    glDeleteProgram(shader_program_);
}

void Scene::render(float peak) {
    glUseProgram(shader_program_);
    glBindVertexArray(vao_);

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, rotation_[0], glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, rotation_[1], glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, rotation_[2], glm::vec3(0.0f, 0.0f, 1.0f));

    glm::vec3 scale{scale_[0], scale_[1], scale_[2]};
    model = glm::scale(model, scale);

    glm::vec3 position{position_[0], position_[1], position_[2]};
    view = glm::translate(view, position);

    projection =
        glm::perspective(glm::radians(45.0f), 1600.0f / 1200.0f, 0.1f, 100.0f);

    unsigned int model_loc = glGetUniformLocation(shader_program_, "model");
    unsigned int view_loc = glGetUniformLocation(shader_program_, "view");
    unsigned int projection_loc =
        glGetUniformLocation(shader_program_, "projection");
    unsigned int time_loc = glGetUniformLocation(shader_program_, "time");

    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projection_loc, 1, GL_FALSE, &projection[0][0]);
    glUniform1f(time_loc, peak);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}