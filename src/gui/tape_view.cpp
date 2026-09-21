#include "gui/tape_view.hpp"
#include "imgui.h"

TapeView::TapeView() {}

void TapeView::render(AudioEngine& audio_engine, bool* show) {
    ImGui::Begin("Tapes", show);

    ImGui::End();
}
