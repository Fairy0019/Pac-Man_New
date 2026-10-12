#include "audio_manager.h"

#include "raylib.h"

static Music backgroundMusic = {0};
static bool audioDeviceReady = false;
static bool backgroundMusicReady = false;

bool AudioManager_Init(const char *backgroundMusicPath)
{
    if (audioDeviceReady)
        AudioManager_Shutdown();

    InitAudioDevice();
    audioDeviceReady = IsAudioDeviceReady();
    if (!audioDeviceReady)
        return false;

    if (backgroundMusicPath == NULL || backgroundMusicPath[0] == '\0')
    {
        AudioManager_Shutdown();
        return false;
    }

    backgroundMusic = LoadMusicStream(backgroundMusicPath);
    backgroundMusicReady = backgroundMusic.ctxData != NULL;
    if (!backgroundMusicReady)
    {
        AudioManager_Shutdown();
        return false;
    }

    backgroundMusic.looping = true;
    PlayMusicStream(backgroundMusic);
    return true;
}

void AudioManager_Update(void)
{
    if (backgroundMusicReady)
        UpdateMusicStream(backgroundMusic);
}

void AudioManager_SetMusicVolume(float volume)
{
    if (!backgroundMusicReady)
        return;

    if (volume < 0.0f)
        volume = 0.0f;
    else if (volume > 1.0f)
        volume = 1.0f;

    SetMusicVolume(backgroundMusic, volume);
}

void AudioManager_Shutdown(void)
{
    if (backgroundMusicReady)
    {
        StopMusicStream(backgroundMusic);
        UnloadMusicStream(backgroundMusic);
        backgroundMusicReady = false;
    }

    if (audioDeviceReady)
    {
        CloseAudioDevice();
        audioDeviceReady = false;
    }
}
