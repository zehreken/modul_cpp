#include "gui/mixer_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void MixerView::render(AudioEngine& audio_engine) {
    ImGui::Begin("Mixer");

    ImGui::Text("Selected tape: %d", audio_engine.get_selected_tape() + 1);

    constexpr float width = 80.0f;
    for (int i = 0; i < 8; ++i) {
        ImGui::PushID(i);
        ImGui::BeginChild("##Child", ImVec2{width, 500.0f});
        ImGui::Text("Tape: %d", i + 1);
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

    ImGui::End();
}