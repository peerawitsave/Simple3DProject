#pragma once
#include <string>

class SoundSystem
{
public:
    bool init();
    void shutdown();

    // Plays a simple test tone using Windows Beep as a fallback.
    void playBeep(int frequencyHz = 440, int durationMs = 200);

    // Attempts to play a WAV file via PlaySound (non-blocking). Returns true if accepted.
    bool playWavFile(const std::string& path, bool loop = false);
    void stop();
};

