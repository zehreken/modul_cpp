#include "gui/scope_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

ScopeView::ScopeView() { fft_input_.resize(128); }

void ScopeView::render(AudioEngine& audio_engine, bool* show) {
    ImGui::Begin("Oscillator Controls", show);

    ImGui::Text("Oscilloscope Output:");
    float* frame_buffer = audio_engine.get_scope_buffer();
    float left[AudioEngine::BUFFER_SIZE]{0.0f};
    float right[AudioEngine::BUFFER_SIZE]{0.0};
    for (int i = 0; i < AudioEngine::BUFFER_SIZE; ++i) {
        left[i] = frame_buffer[i * 2];
        right[i] = frame_buffer[i * 2 + 1];
    }
    ImGui::PlotLines(
        "##Left",
        left,
        AudioEngine::BUFFER_SIZE,
        0,
        nullptr,
        -1.0f,
        1.0f,
        ImVec2(0, 150)
    );
    ImGui::PlotLines(
        "##Right",
        right,
        AudioEngine::BUFFER_SIZE,
        0,
        nullptr,
        -1.0f,
        1.0f,
        ImVec2(0, 150)
    );

    ImGui::Separator();

    for (int i = 0; i < 128; ++i) {
        fft_input_[i] = left[i];
    }

    dj::fft_arg<float> result;
    result = dj::fft1d(fft_input_, dj::fft_dir::DIR_FWD);
    for (int i = 0; i < 128; ++i) {
        right[i] = std::abs(result[i]);
    }
    ImGui::PlotLines(
        "##Frequency Domain",
        right,
        AudioEngine::BUFFER_SIZE,
        0,
        nullptr,
        -1.0f,
        1.0f,
        ImVec2(0, 150)
    );

    ImGui::Text("Left Channel:");
    float recording_buffer[48000]{0.0f};
    audio_engine.copy_recording(recording_buffer, 48000);
    ImGui::PlotLines(
        "##Recording",
        recording_buffer,
        AudioEngine::BUFFER_SIZE,
        0,
        nullptr,
        -1.0f,
        1.0f,
        ImVec2(0, 150)
    );

    ImGui::End();
}