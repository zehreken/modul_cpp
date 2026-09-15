#pragma once

struct VirtualScreen {
    int id_;
    float position_[3]{0.0f, 0.0f, 0.0f};
    float rotation_[3]{0.0f, 0.0f, 0.0f};
    float scale_[3]{1.0f, 1.0f, 1.0f};
};

class Scene {
  public:
    Scene();
    ~Scene();
    void render(float peak);
    float position_[3]{0.0f, 0.0f, 0.0f};
    float rotation_[3]{0.0f, 0.0f, 0.0f};
    float scale_[3]{1.0f, 1.0f, 1.0f};

  private:
    unsigned int vbo_, vao_, ebo_;
    unsigned int shader_program_;
    int transform_loc_;
    VirtualScreen screens_[5];
};
