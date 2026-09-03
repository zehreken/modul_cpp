#include "gui/tapes_view.hpp"
#include "core/audio_engine.hpp"
#include "imgui.h"

void TapesView::render(AudioEngine& audio_engine) {
    ImGui::Begin("Tapes");

    ImGui::Text("Selected tape: %d", audio_engine.get_selected_tape() + 1);

    ImGui::End();
}