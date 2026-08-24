#include "gui/device_view.hpp"
#include "audio_engine.hpp"
#include "imgui.h"

void DeviceView::render(AudioEngine &audio_engine)
{
    ImGui::Begin("Device Settings");

    auto playback_devices = audio_engine.get_playback_devices();
    if (!playback_devices.empty())
    {
        const char *current_label = playback_devices[selected_playback_device_id_].name.c_str();

        if (ImGui::BeginCombo("Output Device", current_label))
        {
            for (size_t i = 0; i < playback_devices.size(); ++i)
            {
                bool is_selected = (selected_playback_device_id_ == static_cast<int>(i));
                if (ImGui::Selectable(playback_devices[i].name.c_str(), is_selected))
                {
                    selected_playback_device_id_ = static_cast<int>(i);
                    audio_engine.select_playback_device(selected_playback_device_id_);
                }
            }
            ImGui::EndCombo();
        }
    }

    auto capture_devices = audio_engine.get_capture_devices();
    if (!capture_devices.empty())
    {
        const char *current_label = capture_devices[selected_capture_device_id_].name.c_str();

        if (ImGui::BeginCombo("Input Devices", current_label))
        {
            for (size_t i = 0; i < capture_devices.size(); ++i)
            {
                bool is_selected = (selected_capture_device_id_ == static_cast<int>(i));
                if (ImGui::Selectable(capture_devices[i].name.c_str(), is_selected))
                {
                    selected_capture_device_id_ = static_cast<int>(i);
                    audio_engine.select_capture_device(selected_capture_device_id_);
                }
            }
            ImGui::EndCombo();
        }
    }

    ImGui::End();
}