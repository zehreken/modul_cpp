#include "gui/metronome_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void MetronomeView::render(AudioEngine& audio_engine) {
    ImGui::Begin("Metronome");

    if (ImGui::Checkbox("Run", &can_metronome_run_)) {
        audio_engine.toggle_metronome();
    }

    ImGui::End();
}