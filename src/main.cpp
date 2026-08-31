#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "core/audio_engine.hpp"
#include "gui/main_view.hpp"

void key_callback(
    GLFWwindow* window, int key, int scancode, int action, int mods
) {
    auto* audio_system =
        static_cast<AudioEngine*>(glfwGetWindowUserPointer(window));
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        audio_system->toggle_play_through();
    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        audio_system->toggle_record();
    }
}

int main() {
    if (!glfwInit())
        return -1;

#if __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window =
        glfwCreateWindow(1024, 600, "modul ❤", nullptr, nullptr);
    if (!window) {
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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

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