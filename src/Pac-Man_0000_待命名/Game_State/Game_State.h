#pragma once

#ifndef GAME_STATE_H
#define GAME_STATE_H

typedef enum
{
    STATE_PROFILE,
    STATE_MAIN_MENU,
    STATE_MODE_SELECT,
    STATE_GAME,
    STATE_PAUSE,
    STATE_HISTORY,
    STATE_SETTING,
    STATE_EXIT,
    MAX_STATES
} GameState;

typedef void (*StateCallback)(void);

extern GameState currentState;

void setGameState(GameState newState);
GameState getGameState(void);
void registerEnterCallback(GameState state, StateCallback callback);
void registerExitCallback(GameState state, StateCallback callback);

#endif
