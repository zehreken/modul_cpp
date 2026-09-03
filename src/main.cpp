#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

#include "core/audio_engine.hpp"
#include "gui/main_view.hpp"
#include "renderer/scene.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods
);

int main() {
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    GLFWwindow* window =
        glfwCreateWindow(1024, 600, "modul ❤", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }

    float scale_x, scale_y;
    glfwGetWindowContentScale(
        window,
        &scale_x,
        &scale_y
    ); // It is 1.5 for my screen
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(scale_x);
    style.FontScaleDpi = scale_x;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    AudioEngine audio_engine{192000};
    audio_engine.init();

    // Keyboard input
    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowUserPointer(window, &audio_engine);
    double elapsed_time = glfwGetTime();

    MainView main_view(audio_engine);
    Scene scene;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // FIX: Calculate delta time, unused atm
        double delta_time = glfwGetTime() - elapsed_time;
        elapsed_time = glfwGetTime();
        main_view.render(audio_engine);

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(1.0f, 0.0f, 0.33f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        scene.render();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // Cleanup
    // audio_engine.shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods
) {
    auto* audio_engine =
        static_cast<AudioEngine*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        audio_engine->toggle_play_through();
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        audio_engine->toggle_record();
    }
    if (key == GLFW_KEY_1 && action == GLFW_PRESS) {
        audio_engine->set_selected_tape(0);
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS) {
        audio_engine->set_selected_tape(1);
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS) {
        audio_engine->set_selected_tape(2);
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS) {
        audio_engine->set_selected_tape(3);
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS) {
        audio_engine->set_selected_tape(4);
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS) {
        audio_engine->set_selected_tape(5);
    }
    if (key == GLFW_KEY_7 && action == GLFW_PRESS) {
        audio_engine->set_selected_tape(6);
    }
    if (key == GLFW_KEY_8 && action == GLFW_PRESS) {
        audio_engine->set_selected_tape(7);
    }
}