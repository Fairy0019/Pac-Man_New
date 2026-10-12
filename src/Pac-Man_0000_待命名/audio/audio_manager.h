#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <stdbool.h>

bool AudioManager_Init(const char *backgroundMusicPath);
void AudioManager_Update(void);
void AudioManager_SetMusicVolume(float volume);
void AudioManager_Shutdown(void);

#endif
