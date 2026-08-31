#pragma once

class Scene {

  public:
    Scene();
    ~Scene();
    void render();

  private:
    unsigned int vbo_, vao_, ebo_;
    unsigned int shader_program_;
};
