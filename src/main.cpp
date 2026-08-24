#define MINIAUDIO_IMPLEMENTATION

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>

#include "audio_engine.hpp"

int main()
{
    if (!glfwInit())
        return -1;

#if __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow *window = glfwCreateWindow(1024, 600, "modul ❤", nullptr, nullptr);
    float scaleX, scaleY;
    glfwGetWindowContentScale(window, &scaleX, &scaleY); // It is 1.5 for my screen

    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(2.0f);
    style.FontScaleDpi = 2.0f;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 150");

    AudioEngine audio_engine;
    audio_engine.init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New", "Ctrl + N"))
                {
                }
                if (ImGui::MenuItem("Open", "Ctrl + O"))
                {
                }
                if (ImGui::MenuItem("Save", "Ctrl + S"))
                {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        ImGui::Begin("Oscillator Controls");

        float freq = audio_engine.get_frequency();

        if (ImGui::SliderFloat("Frequency (Hz)", &freq, 20.0f, 2000.0f, "%.1f Hz"))
        {
            audio_engine.set_frequency(freq);
        }

        float vol = audio_engine.get_volume();
        if (ImGui::SliderFloat("Volume", &vol, 0.0f, 1.0f, "%.2f"))
        {
            audio_engine.set_volume(vol);
        }

        ImGui::Separator();

        ImGui::Text("Oscilloscope Output:");
        float display_buffer_[512]{0.0f};
        audio_engine.copy_scope_buffer(display_buffer_, AudioEngine::SCOPE_SIZE);
        ImGui::PlotLines("##Waveform", display_buffer_, AudioEngine::SCOPE_SIZE, 0, nullptr, -1.0f, 1.0f, ImVec2(0, 150));

        ImGui::End();

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
    audio_engine.shutdown();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}