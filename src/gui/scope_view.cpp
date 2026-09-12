#include "gui/scope_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void ScopeView::render(AudioEngine& audio_engine, bool* show) {
    ImGui::Begin("Oscillator Controls", show);

    ImGui::Separator();

    ImGui::Text("Oscilloscope Output:");
    float display_buffer[AudioEngine::SCOPE_SIZE]{0.0f};
    audio_engine.copy_scope_buffer(display_buffer, AudioEngine::SCOPE_SIZE);
    ImGui::PlotLines(
        "##Waveform",
        display_buffer,
        AudioEngine::SCOPE_SIZE,
        0,
        nullptr,
        -1.0f,
        1.0f,
        ImVec2(0, 150)
    );

    ImGui::Separator();

    ImGui::Text("Left Channel:");
    float recording_buffer[48000]{0.0f};
    audio_engine.copy_recording(recording_buffer, 48000);
    ImGui::PlotLines(
        "##Recording",
        recording_buffer,
        AudioEngine::SCOPE_SIZE,
        0,
        nullptr,
        -1.0f,
        1.0f,
        ImVec2(0, 150)
    );

    ImGui::End();
}