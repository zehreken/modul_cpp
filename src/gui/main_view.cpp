#include "gui/main_view.hpp"
#include "imgui.h"

MainView::MainView(AudioEngine& audio_engine) : device_view_(audio_engine) {}

void MainView::render(AudioEngine& audio_engine) {
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl + N")) {
            }
            if (ImGui::MenuItem("Open", "Ctrl + O")) {
            }
            if (ImGui::MenuItem("Save", "Ctrl + S")) {
            }
            ImGui::EndMenu();
        }
        if (audio_engine.can_record()) {
            ImGui::Text("Recording!");
        }
        if (audio_engine.can_play_through()) {
            ImGui::Text("Playing through");
        }
        ImGui::EndMainMenuBar();
    }

    device_view_.render(audio_engine);
    scope_view_.render(audio_engine);
    tapes_view_.render(audio_engine);
}