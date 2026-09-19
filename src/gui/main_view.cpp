#include "gui/main_view.hpp"
#include "imgui.h"

MainView::MainView(AudioEngine& audio_engine) : stats_view_(audio_engine) {}

void MainView::render(AudioEngine& audio_engine, Scene& scene) {
    bool show_new_project_modal = false;
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("New", "Ctrl + N")) {
                show_new_project_modal = true;
                audio_devices_ = audio_engine.get_audio_devices();
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
            if (ImGui::MenuItem("Renderers")) {
                show_render_view_ = !show_render_view_;
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
        ImGui::Separator();
        render_project_popup(audio_engine);
        if (ImGui::Button("OK")) {
            audio_engine.init(project_config_);
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    if (show_devices_)
        stats_view_.render(audio_engine, &show_devices_);
    if (show_scope_)
        scope_view_.render(audio_engine, &show_scope_);
    if (show_mixer_)
        mixer_view_.render(audio_engine, &show_mixer_);
    if (show_metronome_)
        metronome_view_.render(audio_engine, &show_metronome_);
    if (show_render_view_)
        render_view_.render(scene, &show_render_view_);
}

void MainView::render_project_popup(AudioEngine& audio_engine) {
    {
        ImGui::Text("Playback Device");
        ImGui::Text(audio_devices_
                        .playback_devices_[selected_playback_device_id_]
                        .name_.c_str());
        ImGui::Text(
            "Channels: %d",
            audio_devices_.playback_devices_[selected_playback_device_id_]
                .channels_
        );
        ImGui::Text(
            "Format: %d",
            audio_devices_.playback_devices_[selected_playback_device_id_]
                .format_
        );
        ImGui::Text(
            "Sample Rate: %d",
            audio_devices_.playback_devices_[selected_playback_device_id_]
                .sample_rate_
        );
    }

    ImGui::Separator();
    {
        ImGui::Text("Capture Device");
        ImGui::Text(audio_devices_.capture_devices_[selected_capture_device_id_]
                        .name_.c_str());
        ImGui::Text(
            "Channels: %d",
            audio_devices_.capture_devices_[selected_capture_device_id_]
                .channels_
        );
        ImGui::Text(
            "Format: %d",
            audio_devices_.capture_devices_[selected_capture_device_id_].format_
        );
        ImGui::Text(
            "Sample Rate: %d",
            audio_devices_.capture_devices_[selected_capture_device_id_]
                .sample_rate_
        );
    }

    if (ImGui::Button("Refresh Devices")) {
        audio_devices_ = audio_engine.get_audio_devices();
    }
    if (!audio_devices_.playback_devices_.empty()) {
        const char* current_label =
            audio_devices_.playback_devices_[selected_playback_device_id_]
                .name_.c_str();

        if (ImGui::BeginCombo("Output Device", current_label)) {
            for (size_t i = 0; i < audio_devices_.playback_devices_.size();
                 ++i) {
                bool is_selected =
                    (selected_playback_device_id_ == static_cast<int>(i));
                if (ImGui::Selectable(
                        audio_devices_.playback_devices_[i].name_.c_str(),
                        is_selected
                    )) {
                    selected_playback_device_id_ = static_cast<int>(i);
                    audio_engine.select_devices(
                        selected_playback_device_id_,
                        selected_capture_device_id_
                    );
                }
            }
            ImGui::EndCombo();
        }
    }

    if (!audio_devices_.capture_devices_.empty()) {
        const char* current_label =
            audio_devices_.capture_devices_[selected_capture_device_id_]
                .name_.c_str();

        if (ImGui::BeginCombo("Input Devices", current_label)) {
            for (size_t i = 0; i < audio_devices_.capture_devices_.size();
                 ++i) {
                bool is_selected =
                    (selected_capture_device_id_ == static_cast<int>(i));
                if (ImGui::Selectable(
                        audio_devices_.capture_devices_[i].name_.c_str(),
                        is_selected
                    )) {
                    selected_capture_device_id_ = static_cast<int>(i);
                    audio_engine.select_devices(
                        selected_playback_device_id_,
                        selected_capture_device_id_
                    );
                }
            }
            ImGui::EndCombo();
        }
    }

    ImGui::Separator();
    ImGui::InputInt("BPM", &project_config_.bpm_);
    ImGui::InputInt("Bar Count", &project_config_.bar_count_);
}