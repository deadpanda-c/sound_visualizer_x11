#include "audio/AudioCapture.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <csignal>
#include <atomic>

std::atomic<bool> running(true);

void signalHandler(int signal) {
  (void)signal; // Unused parameter
    running = false;
}

int main() {
    signal(SIGINT, signalHandler); // Handle Ctrl+C
    signal(SIGTERM, signalHandler);

    audio::AudioCapture audioCapture("", 44100, 2);

    if (!audioCapture.start()) {
        std::cerr << "Failed to start audio capture!" << std::endl;
        return 1;
    }

    std::cout << "Running... Press Ctrl+C to stop." << std::endl;

    while (running) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    audioCapture.stop();
    std::cout << "Stopped cleanly." << std::endl;
    return 0;
}
