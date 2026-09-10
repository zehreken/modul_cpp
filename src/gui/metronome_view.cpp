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

    float vol = audio_engine.get_volume();
    if (ImGui::SliderFloat("Volume", &vol, 0.0f, 1.0f, "%.2f")) {
        audio_engine.set_volume(vol);
    }

    ImGui::End();
}