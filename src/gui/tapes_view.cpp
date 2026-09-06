#include "gui/tapes_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void TapesView::render(AudioEngine& audio_engine) {
    ImGui::Begin("Tapes");

    ImGui::Text("Selected tape: %d", audio_engine.get_selected_tape() + 1);

    for (int i = 0; i < 8; ++i) {
        ImGui::PushID(i);
        ImGui::Text("Tape: %d", i + 1);
        ImGui::SameLine();
        float volume = audio_engine.get_tape(i).get_volume();
        const ImVec2 v = ImVec2{50.0f, 400.0f};
        if (ImGui::VSliderFloat("##Test", v, &volume, 0.0f, 1.0f, "%.2f")) {
            audio_engine.get_tape(i).set_volume(volume);
        }
        ImGui::SameLine();

        // if (ImGui::SliderFloat(
        //         "##Volume", &volume, 0.0f, 1.0f, "Volume: %.2f"
        //     )) {
        //     audio_engine.get_tape(i).set_volume(volume);
        // }
        // if (ImGui::SliderFloat("##Pan", &volume, -1.0f, 1.0f, "Pan: %.2f")) {
        // }
        ImGui::PopID();
    }

    ImGui::End();
}