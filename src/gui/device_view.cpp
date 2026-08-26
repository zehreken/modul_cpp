#include "gui/device_view.hpp"
#include "audio_engine.hpp"
#include "imgui.h"

DeviceView::DeviceView(AudioEngine& audio_engine)
    : audio_devices_(audio_engine.get_audio_devices()) {}

void DeviceView::render(AudioEngine& audio_engine) {
    ImGui::Begin("Device Settings");

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
                        is_selected)) {
                    selected_playback_device_id_ = static_cast<int>(i);
                    audio_engine.select_devices(selected_playback_device_id_,
                                                selected_capture_device_id_);
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
                        is_selected)) {
                    selected_capture_device_id_ = static_cast<int>(i);
                    audio_engine.select_devices(selected_playback_device_id_,
                                                selected_capture_device_id_);
                }
            }
            ImGui::EndCombo();
        }
    }

    ImGui::End();
}