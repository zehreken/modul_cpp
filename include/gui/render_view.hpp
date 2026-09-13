#pragma once

class Scene; // Forward declaration

class RenderView {
  public:
    RenderView();
    void render(Scene& scene, bool* show);

  private:
};