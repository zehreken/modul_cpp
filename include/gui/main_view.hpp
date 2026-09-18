#pragma once

#include "gui/metronome_view.hpp"
#include "gui/mixer_view.hpp"
#include "gui/render_view.hpp"
#include "gui/scope_view.hpp"
#include "gui/stats_view.hpp"

class AudioEngine;  // Forward declaration
class Scene;        // Forward declaration
class AudioDevices; // Forward declaration

class MainView {
  public:
    MainView(AudioEngine& audio_engine);
    void render_project_popup(AudioEngine& audio_engine);
    void render(AudioEngine& audio_engine, Scene& scene);

  private:
    StatsView stats_view_;
    ScopeView scope_view_;
    MixerView mixer_view_;
    MetronomeView metronome_view_;
    RenderView render_view_;
    bool show_devices_{true};
    bool show_scope_{true};
    bool show_mixer_{true};
    bool show_metronome_{true};
    bool show_render_view_{true};

    AudioDevices audio_devices_;
    int selected_playback_device_id_{0};
    int selected_capture_device_id_{0};
};