#include "gui/tape_view.hpp"
#include "constants.hpp"
#include "imgui.h"

TapeView::TapeView() {}

void TapeView::render(AudioEngine& audio_engine, bool* show) {
    ImGui::Begin("Tapes", show);

    auto selected_tape = audio_engine.get_selected_tape();
    ImGui::Text("Selected tape: %d", selected_tape + 1);
    if (ImGui::Button("Clear")) {
        audio_engine.get_tape(selected_tape).clear();
    }
    int frame_index = audio_engine.get_frame_index() % 192000;
    ImGui::SliderInt("Tape", &frame_index, 0, 192000, "%1");

    for (int i = 0; i < 8; ++i) {
        ImGui::PushID(i);
        // ImGui::BeginChild("##Tape")

        float* tape_data = audio_engine.get_view_tape(i).data();
        ImGui::PlotLines(
            "##Tape",
            tape_data,
            constants::TAPE_VIEW_SIZE,
            0,
            nullptr,
            -1.0f,
            1.0f,
            ImVec2(0, 150)
        );

        ImGui::PopID();
    }

    ImGui::End();
}
