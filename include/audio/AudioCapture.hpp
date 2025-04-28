#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <thread>
#include <vector>

// libpulse
#include <pulse/pulseaudio.h>
#include <pulse/simple.h>

namespace audio {
  class AudioCapture {
  public:
    AudioCapture(const std::string& deviceName, int sampleRate, int channels);
    ~AudioCapture();

    bool start();
    void stop();
    bool isRunning() const;

    void setDeviceName(const std::string& deviceName);
    std::string getDeviceName() const;

  private:
    std::string _deviceName;
    int _sampleRate;
    int _channels;
    pa_simple* _paSimple;
    pa_sample_spec _sampleSpec;
    bool _running;
    std::thread _captureThread;

    void init();
    void cleanup();

    void captureLoop();
  };
}

