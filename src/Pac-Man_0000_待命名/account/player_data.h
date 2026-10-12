#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <stdbool.h>

typedef struct
{
    int highScore;
    int gamesPlayed;
} PlayerData;

bool PlayerData_Init(void);
bool PlayerData_Create(PlayerData *data);
bool PlayerData_Save(const PlayerData *data, const char *uid);
bool PlayerData_Load(PlayerData *data, const char *uid);
bool PlayerData_Delete(const char *uid);
bool PlayerData_Exists(const char *uid);
void PlayerData_Shutdown(void);

#endif
