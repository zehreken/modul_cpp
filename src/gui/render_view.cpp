#include "gui/render_view.hpp"
#include "imgui.h"
#include "renderer/scene.hpp"

RenderView::RenderView() {}

void RenderView::render(Scene& scene, bool* show) {
    ImGui::Begin("Renderers", show);

    ImGui::SliderFloat3("Position", scene.position_, -5.0f, 0.0f, "%.2f");
    ImGui::SliderFloat3("Rotation", scene.rotation_, -1.0f, 1.0f, "%.2f");
    ImGui::SliderFloat3("Scale", scene.scale_, -1.0f, 1.0f, "%.2f");

    ImGui::End();
}
