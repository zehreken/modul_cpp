#include "gui/controls_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void ControlsView::render(AudioEngine& audio_engine, bool* show) {
    ImGui::Begin("Controls", show);

    if (ImGui::Button("T")) {
        audio_engine.toggle_play_through();
    }
    ImGui::SameLine();
    ImGui::Text("Toggle play-through");

    if (ImGui::Button("R")) {
        audio_engine.toggle_record();
    }
    ImGui::SameLine();
    ImGui::Text("Toggle record");

    if (ImGui::Button("W")) {
        audio_engine.toggle_write_disk();
    }
    ImGui::SameLine();
    ImGui::Text("Toggle write to disk");

    ImGui::End();
}