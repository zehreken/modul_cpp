#pragma once

#include "gui/metronome_view.hpp"
#include "gui/mixer_view.hpp"
#include "gui/render_view.hpp"
#include "gui/scope_view.hpp"
#include "gui/stats_view.hpp"
#include "gui/tape_view.hpp"

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
    TapeView tape_view_;
    bool show_devices_view_{false};
    bool show_scope_view_{false};
    bool show_mixer_view_{true};
    bool show_metronome_view_{true};
    bool show_render_view_{true};
    bool show_tape_view_{true};

    ProjectConfig project_config_;
    AudioDevices audio_devices_;
    int selected_playback_device_id_{0};
    int selected_capture_device_id_{0};
};