#include "gui/main_view.hpp"
#include "imgui.h"

MainView::MainView(AudioEngine& audio_engine) : device_view_(audio_engine) {}

void MainView::render(AudioEngine& audio_engine) {
    bool show_new_project_modal = false;
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl + N")) {
                show_new_project_modal = true;
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

    if (show_new_project_modal) {
        ImGui::OpenPopup("New Project");
    }
    if (ImGui::BeginPopupModal(
            "New Project", nullptr, ImGuiWindowFlags_AlwaysAutoResize
        )) {
        ImGui::Text("Lorem ipsum");
        ImGui::Separator();
        if (ImGui::Button("OK")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    device_view_.render(audio_engine);
    scope_view_.render(audio_engine);
    mixer_view_.render(audio_engine);
    metronome_view_.render(audio_engine);
}
