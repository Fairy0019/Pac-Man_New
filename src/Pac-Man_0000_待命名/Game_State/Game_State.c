#include "Game_State.h"

//默认进入玩家档案
GameState currentState = STATE_PROFILE;

//修改状态
void setGameState(GameState state)
{
    currentState = state;
}

//获取当前状态
GameState getGameState(void)
{
    return currentState;
}
