#ifndef HISTORY_H
#define HISTORY_H

#include <time.h>

typedef struct
{
    int mode;
    int score;
    time_t startTime;
    int durationSeconds;
    int result;
} GameHistory;

int History_Init(void);
void History_Shutdown(void);
int History_Add(const char *uid, const GameHistory *record);
int History_LoadAll(const char *uid, GameHistory **records);

#endif
