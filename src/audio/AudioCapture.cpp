#include "audio/AudioCapture.hpp"

audio::AudioCapture::AudioCapture(const std::string &deviceName, int sampleRate, int channels)
    : _deviceName(deviceName), _sampleRate(sampleRate), _channels(channels)
{
  _running = false;
}

audio::AudioCapture::~AudioCapture()
{
  stop();
}

bool audio::AudioCapture::start()
{
  if (_running)
    return false;

  _running = true;
  _captureThread = std::thread(&AudioCapture::captureLoop, this);

  return true;
}

void audio::AudioCapture::stop()
{
  if (!_running)
    return;

  _running = false;
  if (_captureThread.joinable())
    _captureThread.join();
}

bool audio::AudioCapture::isRunning() const
{
  return _running;
}

void audio::AudioCapture::setDeviceName(const std::string &deviceName)
{
  _deviceName = deviceName;
}

std::string audio::AudioCapture::getDeviceName() const
{
  return _deviceName;
}

void audio::AudioCapture::init()
{
  // Initialize the audio capture device here
  // This is a placeholder for actual initialization code
  std::cout << "Initializing audio capture device: " << _deviceName << std::endl;

  // Example: Initialize PulseAudio
  pa_sample_spec ss;
  ss.format = PA_SAMPLE_S16LE;
  ss.rate = _sampleRate;
  ss.channels = _channels;

  int error;
  _paSimple = pa_simple_new(NULL, _deviceName.c_str(), PA_STREAM_RECORD, NULL, "AudioCapture", &ss, NULL, NULL, &error);
  if (!_paSimple) {
    std::cerr << "pa_simple_new() failed: " << pa_strerror(error) << std::endl;
    return;
  }
}

void audio::AudioCapture::cleanup()
{
  // Cleanup the audio capture device here
  // This is a placeholder for actual cleanup code
  std::cout << "Cleaning up audio capture device: " << _deviceName << std::endl;
}

void audio::AudioCapture::captureLoop()
{
    init();

    const size_t bufferSize = 4096; // Size of the buffer to read each time (bytes)
    std::vector<uint8_t> buffer(bufferSize);

    std::cout << "Starting audio capture..." << std::endl;

    while (_running) {

        int error = 0;

        // Read raw audio data from PulseAudio into buffer
        if (pa_simple_read(_paSimple, buffer.data(), buffer.size(), &error) < 0) {
            std::cerr << "pa_simple_read() failed: " << pa_strerror(error) << std::endl;
            break;
        }
        std::cout << "Read " << buffer.size() << " bytes from audio device." << std::endl;

        // Example: compute simple volume level (amplitude)
        int64_t sum = 0;
        int16_t* samples = reinterpret_cast<int16_t*>(buffer.data());
        size_t sampleCount = buffer.size() / sizeof(int16_t);

        for (size_t i = 0; i < sampleCount; ++i) {
            sum += std::abs(samples[i]);
        }

        int averageAmplitude = static_cast<int>(sum / sampleCount);

        // Print the amplitude (later you will send it to FFT)
        std::cout << "Average Amplitude: " << averageAmplitude << std::endl;

        // Small sleep to avoid spamming too hard
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    cleanup();
}
