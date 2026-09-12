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
        if (ImGui::BeginMenu("Windows")) {
            if (ImGui::MenuItem("Device")) {
                show_devices_ = !show_devices_;
            }
            if (ImGui::MenuItem("Scope")) {
                show_scope_ = !show_scope_;
            }
            if (ImGui::MenuItem("Mixer")) {
                show_mixer_ = !show_mixer_;
            }
            if (ImGui::MenuItem("Metronome")) {
                show_metronome_ = !show_metronome_;
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

    if (show_devices_)
        device_view_.render(audio_engine, &show_devices_);
    if (show_scope_)
        scope_view_.render(audio_engine, &show_scope_);
    if (show_mixer_)
        mixer_view_.render(audio_engine, &show_mixer_);
    if (show_metronome_)
        metronome_view_.render(audio_engine, &show_metronome_);
}
