#include "gui/tape_view.hpp"
#include "imgui.h"

TapeView::TapeView() {}

void TapeView::render(AudioEngine& audio_engine, bool* show) {
    ImGui::Begin("Tapes", show);

    int frame_index = audio_engine.get_frame_index() % 192000;
    ImGui::SliderInt("Tape", &frame_index, 0, 192000, "%1");

    ImGui::End();
}
