#include "Game_State.h"

#include <stdbool.h>
#include <stddef.h>

// 主程序使用唯一的 GameState 驱动菜单、设置和游戏页面。
GameState currentState = STATE_PROFILE;

static StateCallback enterCallbacks[MAX_STATES] = {0};
static StateCallback exitCallbacks[MAX_STATES] = {0};

static bool GameState_IsValid(GameState state)
{
    return state >= STATE_PROFILE && state < MAX_STATES;
}

static bool GameState_CanChange(GameState from, GameState to)
{
    if (!GameState_IsValid(from) || !GameState_IsValid(to))
        return false;

    if (to == STATE_EXIT)
        return true;

    switch (from)
    {
    case STATE_PROFILE:
        return to == STATE_MAIN_MENU;

    case STATE_MODE_SELECT:
        return to == STATE_MAIN_MENU || to == STATE_GAME;

    case STATE_SETTING:
    case STATE_HISTORY:
        return to == STATE_MAIN_MENU;

    case STATE_MAIN_MENU:
        return to == STATE_MODE_SELECT ||
               to == STATE_SETTING ||
               to == STATE_HISTORY;

    case STATE_GAME:
        return to == STATE_PAUSE || to == STATE_MAIN_MENU;

    case STATE_PAUSE:
        return to == STATE_GAME ||
               to == STATE_MAIN_MENU ||
               to == STATE_MODE_SELECT;

    case STATE_EXIT:
    case MAX_STATES:
        return false;
    }

    return false;
}

void registerEnterCallback(GameState state, StateCallback callback)
{
    if (GameState_IsValid(state))
        enterCallbacks[state] = callback;
}

void registerExitCallback(GameState state, StateCallback callback)
{
    if (GameState_IsValid(state))
        exitCallbacks[state] = callback;
}

void setGameState(GameState newState)
{
    if (newState == currentState || !GameState_CanChange(currentState, newState))
        return;

    if (exitCallbacks[currentState] != NULL)
        exitCallbacks[currentState]();

    currentState = newState;

    if (enterCallbacks[currentState] != NULL)
        enterCallbacks[currentState]();
}

GameState getGameState(void)
{
    return currentState;
}
