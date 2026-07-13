#include <stdio.h>
#include <string.h>
#include "Game_State.h"
#include <stdbool.h>

bool gameRunning = true;

// ---------- 显示当前状态 ----------
static void drawState(void)
{
    const char *stateNames[] = {
        [STATE_PROFILE] = "个人资料",
        [STATE_MAIN_MENU] = "主菜单",

        [STATE_MODE_SELECT] = "模式选择",

        [STATE_GAME] = "游戏",
        [STATE_PAUSE] = "暂停",

        [STATE_HISTORY] = "历史记录",
        [STATE_SETTING] = "设置",

        [STATE_EXIT] = "退出"};
    printf("\n==========\n");
    printf("当前状态: %s\n", stateNames[getGameState()]);
    printf("==========\n");
}

void showAvailableTransitions()
{
    printf("\n=====================\n");

    switch (currentState)
    {

    case STATE_PROFILE:

        printf("当前状态：个人档案\n");
        printf("1. 进入主菜单\n");
        printf("0. 退出程序\n");

        break;

    case STATE_MAIN_MENU:

        printf("当前状态：主菜单\n");
        printf("1. 开始游戏(模式选择)\n");
        printf("2. 设置\n");
        printf("3. 历史记录\n");
        printf("0. 退出程序\n");

        break;

    case STATE_MODE_SELECT:

        printf("1.普通模式\n");
        printf("2.经典回顾（经典四鬼）\n");
        printf("······\n");
        printf("设计中\n");
        printf("······\n");
        printf("3.返回菜单\n");

        break;

    case STATE_GAME:

        printf("当前状态：游戏中\n");
        printf("1. 暂停\n");


        break;

    case STATE_PAUSE:

        printf("当前状态：暂停\n");
        printf("1. 返回游戏\n");
        printf("2. 返回菜单\n");
        printf("3. 返回模式选择\n");

        break;

    case STATE_HISTORY:

        printf("当前状态：历史记录\n");
        printf("1. 返回菜单\n");

        break;

    case STATE_SETTING:

        printf("当前状态：设置\n");
        printf("1. 返回菜单\n");

        break;
    }

    printf("=====================\n");
}

// ---------- 输入处理（绝不会退出或死循环） ----------
void handleInput()
{
    int choice;

    showAvailableTransitions();

    scanf("%d", &choice);

    switch (currentState)
    {

    case STATE_PROFILE:

        if (choice == 1)
            setGameState(STATE_MAIN_MENU);

        else if (choice == 0)
            gameRunning = false;

        break;

    case STATE_MAIN_MENU:

        if (choice == 1)
            setGameState(STATE_MODE_SELECT);

        else if (choice == 2)
            setGameState(STATE_SETTING);

        else if (choice == 3)
            setGameState(STATE_HISTORY);

        else if (choice == 0)
            gameRunning = false;

        break;

    case STATE_MODE_SELECT:

        if (choice == 1)
            setGameState(STATE_GAME);

        else if (choice == 2)
            setGameState(STATE_GAME);

        else if (choice == 3)
            setGameState(STATE_MAIN_MENU);

        break;

    case STATE_GAME:

        if (choice == 1)
            setGameState(STATE_PAUSE);

        break;

    case STATE_SETTING:

        if (choice == 1)
            setGameState(STATE_MAIN_MENU);

        break;

    case STATE_PAUSE:

        if (choice == 1)
            setGameState(STATE_GAME);

        else if (choice == 2)
            setGameState(STATE_MAIN_MENU);

        else if (choice == 3)
            setGameState(STATE_MODE_SELECT);

        break;

    case STATE_HISTORY:

        if (choice == 1)
            setGameState(STATE_MAIN_MENU);

        break;

    }

}

// ---------- 状态回调示例 ----------
void init()
{
    printf("初始化\n");
}

void shutdown()
{
    printf("关闭程序\n");
}
void enterModeSelect()
{
    printf("进入模式选择\n");
}

void exitModeSelect()
{
    printf("离开模式选择\n");
}
void onEnterGame(void)
{
    printf(" 进入游戏\n");
}
void onExitGame(void)
{
    printf(" 退出游戏\n");
}
#include <stdio.h>

void updateProfile()
{
    printf("更新个人档案\n");
}

void updateMenu()
{
    printf("更新主菜单\n");
}

void updateGame()
{
    printf("更新游戏\n");
}

void updatePause()
{
    printf("更新暂停\n");
}

void drawProfile()
{
    printf("绘制个人档案\n");
}

void drawMenu()
{
    printf("绘制主菜单\n");
}

void drawGame()
{
    printf("绘制游戏画面\n");
}

void drawPause()
{
    printf("绘制暂停画面\n");
}

void updateModeSelect()
{
    printf("更新模式选择\n");
}

void drawModeSelect()
{
    // printf("\n=====================\n");
    // printf("当前状态：游戏模式选择\n");
    // printf("1. 经典模式\n");
    // printf("2. 返回菜单\n");
    // printf("=====================\n");
}

// ---------- 主程序 ----------
int main(void)
{
    // 注册回调
    registerEnterCallback(STATE_GAME, onEnterGame);
    registerExitCallback(STATE_GAME, onExitGame);

    init();

    // 主循环
    while (gameRunning)
    {

        handleInput();

        updateGameState();

        drawGameState();
    }

    shutdown();

    printf("程序正常退出。\n");
    return 0;
}