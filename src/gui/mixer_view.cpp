#include "gui/mixer_view.hpp"
#include "constants.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void MixerView::render(AudioEngine& audio_engine, bool* show) {
    ImGui::Begin("Mixer", show);

    constexpr float width = 80.0f;
    for (int i = 0; i < constants::TAPE_COUNT; ++i) {
        ImGui::PushID(i);
        ImGui::BeginChild("##Child", ImVec2{width, 500.0f});
        ImGui::Text("Tape: %d", i + 1);
        const char* mute_label = audio_engine.is_tape_mute(i) ? "M" : "m";
        if (ImGui::Button(mute_label)) {
            audio_engine.toggle_mute_tape(i);
        }
        ImGui::SameLine();
        const char* solo_label = audio_engine.is_tape_solo(i) ? "S" : "s";
        if (ImGui::Button(solo_label)) {
            audio_engine.toggle_solo_tape(i);
        }
        float pan = audio_engine.get_tape(i).get_pan();
        ImGui::SetNextItemWidth(width);
        if (ImGui::SliderFloat("##Pan", &pan, -1.0f, 1.0f, "%.2f")) {
            audio_engine.get_tape(i).set_pan(pan);
        }
        float volume = audio_engine.get_tape(i).get_volume();
        const ImVec2 size = ImVec2{width, 400.0f};
        if (ImGui::VSliderFloat(
                "##Volume", size, &volume, 0.0f, 1.0f, "%.2f"
            )) {
            audio_engine.get_tape(i).set_volume(volume);
        }
        ImGui::EndChild();
        ImGui::SameLine();
        ImGui::PopID();
    }
    // Master pan and volume
    ImGui::BeginChild("##Child", ImVec2{width, 500.0f});
    ImGui::Text("Master");
    float master_pan = 0.0f;
    ImGui::SetNextItemWidth(width);
    if (ImGui::SliderFloat("##Pan", &master_pan, -1.0f, 1.0f, "%.2f")) {
    }
    float master_volume = audio_engine.get_volume();
    const ImVec2 size = ImVec2{width, 400.0f};
    if (ImGui::VSliderFloat(
            "##Volume", size, &master_volume, 0.0f, 1.0f, "%.2f"
        )) {
        audio_engine.set_volume(master_volume);
    }
    ImGui::EndChild();

    ImGui::End();
}