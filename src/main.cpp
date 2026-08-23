#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <iostream>
#include <cmath>

constexpr float SAMPLE_RATE = 48000.0f;
constexpr size_t SCOPE_SIZE = 512;
constexpr float FREQUENCY = 440.0f;
constexpr float TWO_PI = 6.28318530717958647692f;

struct AudioState
{
    std::atomic<float> frequency{FREQUENCY};
    std::atomic<float> volume{0.2f};
    float phase{0.0f};

    float scope_buffer[SCOPE_SIZE]{0.0f};
    size_t scope_write_index{0};
};

void audio_callback(ma_device *pDevice, void *pOutput, const void *pInput, ma_uint32 frameCount)
{
    auto *state = static_cast<AudioState *>(pDevice->pUserData);
    float *output = static_cast<float *>(pOutput);

    float freq = state->frequency.load(std::memory_order_relaxed);
    float vol = state->volume.load(std::memory_order_relaxed);
    float phase_increment = (TWO_PI * freq) / SAMPLE_RATE;

    for (ma_uint32 i = 0; i < frameCount; ++i)
    {
        float sample = std::sin(state->phase) * vol;

        *output++ = sample; // L
        *output++ = sample; // R

        state->scope_buffer[state->scope_write_index] = sample;
        state->scope_write_index = (state->scope_write_index + 1) % SCOPE_SIZE;

        state->phase += phase_increment;
        if (state->phase >= TWO_PI)
        {
            state->phase -= TWO_PI;
        }
    }

    (void)pInput;
}

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

    AudioState audio_state;
    ma_device_config config = ma_device_config_init(ma_device_type_playback);
    config.playback.format = ma_format_f32;
    config.playback.channels = 2;
    config.sampleRate = static_cast<ma_uint32>(SAMPLE_RATE);
    config.dataCallback = audio_callback;
    config.pUserData = &audio_state;

    ma_device device;
    if (ma_device_init(nullptr, &config, &device) != MA_SUCCESS)
    {
        std::cerr << "Failed to initialize audio device.\n";
        return -1;
    }
    ma_device_start(&device);

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

        float freq = audio_state.frequency.load();

        if (ImGui::SliderFloat("Frequency (Hz)", &freq, 20.0f, 2000.0f, "%.1f Hz"))
        {
            audio_state.frequency.store(freq);
        }

        float vol = audio_state.volume.load();
        if (ImGui::SliderFloat("Volume", &vol, 0.0f, 1.0f, "%.2f"))
        {
            audio_state.volume.store(vol);
        }

        ImGui::Separator();

        ImGui::Text("Oscilloscope Output:");
        ImGui::PlotLines("##Waveform", audio_state.scope_buffer, SCOPE_SIZE, static_cast<int>(audio_state.scope_write_index), nullptr, -1.0f, 1.0f, ImVec2(0, 150));

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
    ma_device_uninit(&device);
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}