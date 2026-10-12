#pragma once

#ifndef MAIN_MENU_H
#define MAIN_MENU_H

// 主菜单返回的操作。页面切换由上层状态管理器负责。
typedef enum
{
    MAIN_MENU_ACTION_NONE = 0,
    MAIN_MENU_ACTION_START_GAME,
    MAIN_MENU_ACTION_HISTORY,
    MAIN_MENU_ACTION_SETTINGS,
    MAIN_MENU_ACTION_EXIT
} MainMenuAction;

// 重置菜单选择状态。
void MainMenu_Init(void);

// 更新菜单输入；没有新操作时返回 MAIN_MENU_ACTION_NONE。
MainMenuAction MainMenu_Update(void);

// 在已打开的 raylib 窗口中绘制主菜单。
void MainMenu_Draw(void);

#endif
