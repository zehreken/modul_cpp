#include "gui/mixer_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void MixerView::render(AudioEngine& audio_engine) {
    ImGui::Begin("Mixer");

    ImGui::Text("Selected tape: %d", audio_engine.get_selected_tape() + 1);

    for (int i = 0; i < 8; ++i) {
        ImGui::BeginGroup();
        ImGui::PushID(i);
        ImGui::Text("Tape: %d", i + 1);
        float volume = audio_engine.get_tape(i).get_volume();
        const ImVec2 v = ImVec2{50.0f, 400.0f};
        if (ImGui::VSliderFloat("##Volume", v, &volume, 0.0f, 1.0f, "%.2f")) {
            audio_engine.get_tape(i).set_volume(volume);
        }
        ImGui::EndGroup();
        ImGui::SameLine();
        ImGui::PopID();
    }

    ImGui::End();
}