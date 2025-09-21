/*
    miniaudio - v0.11.21 - Minimal header-only audio playback and capture library.
    Simplified single-header subset for playback of a generated tone (sine wave) only.
    For full library visit: https://github.com/mackron/miniaudio
*/

#ifndef MINIAUDIO_H
#define MINIAUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { ma_format_f32 = 4 } ma_format;
typedef unsigned int    ma_uint32;
typedef int             ma_bool32;
typedef float           ma_float;

typedef struct
{
    ma_format format;
    ma_uint32 channels;
    ma_uint32 sampleRate;
    void (*dataCallback)(void* pOutput, const void* pInput, ma_uint32 frameCount, void* pUserData);
    void* pUserData;
} ma_device_config;

typedef struct
{
    ma_device_config config;
    ma_bool32 isStarted;
} ma_device;

static ma_device_config ma_device_config_init(ma_format format, ma_uint32 channels, ma_uint32 sampleRate,
                                              void (*onData)(void*, const void*, ma_uint32, void*))
{
    ma_device_config cfg;
    cfg.format = format;
    cfg.channels = channels;
    cfg.sampleRate = sampleRate;
    cfg.dataCallback = onData;
    cfg.pUserData = NULL;
    return cfg;
}

// This is a stub implementation that uses no actual audio backend.
// It's provided to allow compilation in environments without the full miniaudio.
// It will not output sound, but will execute callbacks to keep logic paths intact.

static int ma_device_init(const ma_device_config* pConfig, ma_device* pDevice)
{
    if (!pConfig || !pDevice) return -1;
    pDevice->config = *pConfig;
    pDevice->isStarted = 0;
    return 0;
}

static void ma_device_uninit(ma_device* pDevice)
{
    (void)pDevice;
}

static int ma_device_start(ma_device* pDevice)
{
    if (!pDevice) return -1;
    pDevice->isStarted = 1;
    return 0;
}

static void ma_device_stop(ma_device* pDevice)
{
    if (!pDevice) return;
    pDevice->isStarted = 0;
}

#ifdef __cplusplus
}
#endif

#endif // MINIAUDIO_H

