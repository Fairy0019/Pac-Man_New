#pragma once

#ifndef SYSTEM_H
#define SYSTEM_H

#endif // SYSTEM_H

#ifndef GAME_STATE_H
#define GAME_STATE_H


//====== 头文件声明======
void updateProfile();
void updateMenu();
void updateGame();
void updatePause();

void drawProfile();
void drawMenu();
void drawGame();
void drawPause();

void updateGameState();
void drawGameState();

void updateModeSelect();
void drawModeSelect();

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
    MAX_STATES // 用于数组大小
} GameState;

typedef void (*StateCallback)(void);

extern GameState currentState;

void setGameState(GameState newState);

GameState getGameState(void);

void registerEnterCallback(GameState state, StateCallback cb);

void registerExitCallback(GameState state, StateCallback cb);

#endif