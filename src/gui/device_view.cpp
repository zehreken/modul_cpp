#include "gui/device_view.hpp"
#include "audio_engine.hpp"
#include "imgui.h"

DeviceView::DeviceView(AudioEngine &audio_engine) : audio_devices(audio_engine.get_audio_devices()) {}

void DeviceView::render(AudioEngine &audio_engine)
{
    ImGui::Begin("Device Settings");

    if (ImGui::Button("Refresh Devices"))
    {
        audio_devices = audio_engine.get_audio_devices();
    }
    if (!audio_devices.playback_devices.empty())
    {
        const char *current_label = audio_devices.playback_devices[selected_playback_device_id_].name.c_str();

        if (ImGui::BeginCombo("Output Device", current_label))
        {
            for (size_t i = 0; i < audio_devices.playback_devices.size(); ++i)
            {
                bool is_selected = (selected_playback_device_id_ == static_cast<int>(i));
                if (ImGui::Selectable(audio_devices.playback_devices[i].name.c_str(), is_selected))
                {
                    selected_playback_device_id_ = static_cast<int>(i);
                    audio_engine.select_devices(selected_playback_device_id_, selected_capture_device_id_);
                }
            }
            ImGui::EndCombo();
        }
    }

    if (!audio_devices.capture_devices.empty())
    {
        const char *current_label = audio_devices.capture_devices[selected_capture_device_id_].name.c_str();

        if (ImGui::BeginCombo("Input Devices", current_label))
        {
            for (size_t i = 0; i < audio_devices.capture_devices.size(); ++i)
            {
                bool is_selected = (selected_capture_device_id_ == static_cast<int>(i));
                if (ImGui::Selectable(audio_devices.capture_devices[i].name.c_str(), is_selected))
                {
                    selected_capture_device_id_ = static_cast<int>(i);
                    audio_engine.select_devices(selected_playback_device_id_, selected_capture_device_id_);
                }
            }
            ImGui::EndCombo();
        }
    }

    ImGui::End();
}