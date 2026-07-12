#pragma once

#ifndef MENU_H
#define MENU_H

//菜单状态
typedef enum
{
    MENU_START_GAME = 1,     
    MENU_ACCOUNT,        //账户菜单
    MENU_SETTING,     //设置菜单
    MENU_EXIT         //退出菜单
} MenuChoice;

//显示主菜单
void showMainMenu();

//获取玩家选择
MenuChoice getPlayerChoice();

#endif // MENU_H    