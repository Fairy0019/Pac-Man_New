#include <stdio.h>

#include "Game_State.h"

int main(void)
{
   while(1)
   {
    switch(getGameState())
    {
        case STATE_PROFILE:
            printf("\n");
            printf("==========\n");
            printf("当前状态: 个人资料\n");
            printf("==========\n");
            break;
        
        case STATE_MAIN_MENU:
            printf("\n");
            printf("==========\n");
            printf("当前状态: 主菜单\n");
            printf("==========\n");
            break;

        case STATE_GAME:
            printf("\n");
            printf("==========\n");
            printf("当前状态: 游戏\n");
            printf("==========\n");
            break;

        case STATE_PAUSE:
            printf("\n");
            printf("==========\n");
            printf("当前状态: 暂停\n");
            printf("==========\n");
            break;

        case STATE_HISTORY:
            printf("\n");
            printf("==========\n");
            printf("当前状态: 历史记录\n");
            printf("==========\n");
            break;

        case STATE_SETTING:
            printf("\n");
            printf("==========\n");
            printf("当前状态: 设置\n");
            printf("==========\n");
            break;

        case STATE_EXIT:
            
            return 0;

    }

    printf("\n");
    printf("输入状态:\n");

    int choince;
    scanf("%d", &choince);

    switch(choince)
    {
        case 1:
            setGameState(STATE_PROFILE);
            break;

        case 2:
            setGameState(STATE_MAIN_MENU);
            break;

        case 3:
            setGameState(STATE_GAME);
            break;

        case 4:
            setGameState(STATE_PAUSE);
            break;

        case 5:
            setGameState(STATE_HISTORY);
            break;

        case 6:
            setGameState(STATE_SETTING);
            break;

        case 0:
            setGameState(STATE_EXIT);
            break;
    }

   }

}