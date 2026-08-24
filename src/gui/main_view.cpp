#include "gui/main_view.hpp"
#include "gui/device_view.hpp"
#include "imgui.h"

MainView::MainView(AudioEngine &audio_engine) : device_view_(audio_engine) {}

void MainView::render(AudioEngine &audio_engine)
{
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

    device_view_.render(audio_engine);
    scope_view_.render(audio_engine);
}