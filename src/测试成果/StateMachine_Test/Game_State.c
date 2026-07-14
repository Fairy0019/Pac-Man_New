#include "Game_State.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>


// 定义转换表
typedef struct
{
    int option;
    GameState nextState;
    char text[32];

} Transition;

Transition gameTransitions[] =
    {
        {1, STATE_PAUSE, "暂停"},
        {2, STATE_MAIN_MENU, "返回菜单"}
    };

Transition pauseTransitions[] =
    {
        {1, STATE_GAME, "返回游戏"},
        {2, STATE_MAIN_MENU, "返回菜单"}
    };


// 当前状态初始化为个人资料
 GameState currentState = STATE_PROFILE;

// 回调函数指针数组
static StateCallback enterCallbacks[MAX_STATES] = {0};
static StateCallback exitCallbacks[MAX_STATES] = {0};

// 注册回调
void registerEnterCallback(GameState state, StateCallback cb)
{
    if (state < MAX_STATES)
        enterCallbacks[state] = cb;
}

void registerExitCallback(GameState state, StateCallback cb)
{
    if (state < MAX_STATES)
        exitCallbacks[state] = cb;
}

// 切换状态
bool canChangeState(GameState from, GameState to)
{
    switch (from)
    {
    case STATE_PROFILE:

        return to == STATE_MAIN_MENU;

    case STATE_MAIN_MENU:

        return to == STATE_MODE_SELECT ||
               to == STATE_SETTING ||
               to == STATE_HISTORY ||
               to == STATE_PROFILE;

    case STATE_MODE_SELECT:

        return to == STATE_GAME ||
               to == STATE_MAIN_MENU;

    case STATE_GAME:

        return to == STATE_PAUSE;

    case STATE_PAUSE:

        return to == STATE_GAME ||
               to == STATE_MAIN_MENU ||
               to == STATE_MODE_SELECT;

    case STATE_SETTING:

        return to == STATE_MAIN_MENU;

    case STATE_HISTORY:

        return to == STATE_MAIN_MENU;

    default:
        printf("状态 %d 没有定义转换规则\n", from);
        return false;

    }

}

void setGameState(GameState newState)
{
    printf("切换：%d -> %d\n", currentState, newState);

    if (!canChangeState(currentState, newState))
    {
        printf("非法状态转换\n");
        return;
    }

    if (currentState == newState)
        return;

    if (exitCallbacks[currentState])
        exitCallbacks[currentState]();

    currentState = newState;

    if (enterCallbacks[currentState])
        enterCallbacks[currentState]();
}

// 获取当前状态
GameState getGameState(void)
{
    return currentState;
}

//增加状态函数
typedef struct
{
    void (*update)();
    void (*draw)();

} StateFunction;

void updateGameState()
{
    switch (currentState)
    {

    case STATE_PROFILE:
        updateProfile();
        break;

    case STATE_MAIN_MENU:
        updateMenu();
        break;

    case STATE_MODE_SELECT:

        updateModeSelect();

        break;

    case STATE_GAME:
        updateGame();
        break;

    case STATE_PAUSE:
        updatePause();
        break;
    }
}

void drawGameState()
{
    switch (currentState)
    {

    case STATE_PROFILE:
        drawProfile();
        break;

    case STATE_MAIN_MENU:
        drawMenu();
        break;

    case STATE_MODE_SELECT:

        drawModeSelect();

        break;

    case STATE_GAME:
        drawGame();
        break;

    case STATE_PAUSE:
        drawPause();
        break;
    }
}

const char *stateNames[] =
    {
        "Profile",
        "MainMenu",
        "ModeSelect",
        "Game",
        "Pause",
        "History",
        "Setting"};

void printCurrentState()
{
    printf("\n[State] %s\n", stateNames[currentState]);
}