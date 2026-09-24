#include "gui/metronome_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void MetronomeView::render(AudioEngine& audio_engine, bool* show) {
    ImGui::Begin("Metronome", show);

    if (ImGui::Checkbox("Run", &can_metronome_run_)) {
        audio_engine.toggle_metronome();
    }

    float freq = audio_engine.get_frequency();

    if (ImGui::SliderFloat(
            "Frequency (Hz)", &freq, 20.0f, 2000.0f, "%.1f Hz"
        )) {
        audio_engine.set_frequency(freq);
    }

    ImGui::Text("Beat count: %d", audio_engine.get_beat_count());
    for (int i = 0; i < 4; ++i) {
        ImGui::PushID(i);
        ImGui::RadioButton(
            "##",
            audio_engine.can_metronome_beep() &&
                i == audio_engine.get_beat_count() % 4
        );
        ImGui::SameLine();
        ImGui::PopID();
    }

    ImGui::End();
}