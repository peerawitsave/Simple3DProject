#include "SoundSystem.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
#endif

bool SoundSystem::init()
{
    // No global init needed for simple WinMM usage.
    return true;
}

void SoundSystem::shutdown()
{
    stop();
}

void SoundSystem::playBeep(int frequencyHz, int durationMs)
{
#ifdef _WIN32
    Beep(frequencyHz, durationMs);
#else
    (void)frequencyHz; (void)durationMs;
#endif
}

bool SoundSystem::playWavFile(const std::string& path, bool loop)
{
#ifdef _WIN32
    UINT flags = SND_ASYNC | SND_FILENAME;
    if (loop) flags |= SND_LOOP;
    BOOL ok = PlaySoundA(path.c_str(), NULL, flags);
    return ok == TRUE;
#else
    (void)path; (void)loop; return false;
#endif
}

void SoundSystem::stop()
{
#ifdef _WIN32
    PlaySoundA(NULL, 0, 0); // stop any async sound
#endif
}

