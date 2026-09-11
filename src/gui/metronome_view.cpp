#include "gui/metronome_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void MetronomeView::render(AudioEngine& audio_engine) {
    ImGui::Begin("Metronome");

    if (ImGui::Checkbox("Run", &can_metronome_run_)) {
        audio_engine.toggle_metronome();
    }

    float freq = audio_engine.get_frequency();

    if (ImGui::SliderFloat(
            "Frequency (Hz)", &freq, 20.0f, 2000.0f, "%.1f Hz"
        )) {
        audio_engine.set_frequency(freq);
    }

    ImGui::End();
}