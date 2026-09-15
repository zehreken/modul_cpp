#include "gui/render_view.hpp"
#include "imgui.h"
#include "renderer/scene.hpp"

RenderView::RenderView() {}

void RenderView::render(Scene& scene, bool* show) {
    ImGui::Begin("Renderers", show);

    if (ImGui::Button("Reset Position")) {
        scene.position_[0] = scene.position_[1] = scene.position_[2] = 0.0f;
    }
    ImGui::SliderFloat3("Position", scene.position_, -5.0f, 5.0f, "%.2f");
    if (ImGui::Button("Reset Rotation")) {
        scene.rotation_[0] = scene.rotation_[1] = scene.rotation_[2] = 0.0f;
    }
    ImGui::SliderFloat3("Rotation", scene.rotation_, -1.0f, 1.0f, "%.2f");
    if (ImGui::Button("Reset Scale")) {
        scene.scale_[0] = scene.scale_[1] = scene.scale_[2] = 1.0f;
    }
    ImGui::SliderFloat3("Scale", scene.scale_, 0.1f, 1.0f, "%.2f");

    ImGui::End();
}
