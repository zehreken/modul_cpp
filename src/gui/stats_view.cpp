#include "gui/stats_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

StatsView::StatsView(AudioEngine& audio_engine)
    : audio_devices_(audio_engine.get_audio_devices()) {}

void StatsView::render(AudioEngine& audio_engine, bool* show) {
    ImGui::Begin("Project Stats", show);

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

    ImGui::Separator();

    ImGui::Text("BPM: %d", 120);

    ImGui::End();
}