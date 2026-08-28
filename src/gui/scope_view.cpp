#include "gui/scope_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void ScopeView::render(AudioEngine& audio_engine) {
    ImGui::Begin("Oscillator Controls");

    float freq = audio_engine.get_frequency();

    if (ImGui::SliderFloat("Frequency (Hz)", &freq, 20.0f, 2000.0f,
                           "%.1f Hz")) {
        audio_engine.set_frequency(freq);
    }

    float vol = audio_engine.get_volume();
    if (ImGui::SliderFloat("Volume", &vol, 0.0f, 1.0f, "%.2f")) {
        audio_engine.set_volume(vol);
    }

    ImGui::Separator();

    ImGui::Text("Oscilloscope Output:");
    float display_buffer[AudioEngine::SCOPE_SIZE]{0.0f};
    audio_engine.copy_scope_buffer(display_buffer, AudioEngine::SCOPE_SIZE);
    ImGui::PlotLines("##Waveform", display_buffer, AudioEngine::SCOPE_SIZE, 0,
                     nullptr, -1.0f, 1.0f, ImVec2(0, 150));

    ImGui::End();
}