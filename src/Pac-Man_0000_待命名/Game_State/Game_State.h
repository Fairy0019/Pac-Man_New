#pragma once

#ifndef GAME_STATE_H
#define GAME_STATE_H

//游戏状态
typedef enum
{
    STATE_PROFILE = 0, //个人资料

    STATE_MAIN_MENU,   //主菜单

    STATE_GAME,    //游戏进行中

    STATE_PAUSE,   //游戏暂停

    STATE_HISTORY,  //历史记录

    STATE_SETTING, //设置

    STATE_EXIT //退出游戏

}GameState;

//当前游戏状态（全局变量）
extern GameState currentState;

//设置状态
void setGameState(GameState state);

//获取当前状态
GameState getGameState(void);

#endif 