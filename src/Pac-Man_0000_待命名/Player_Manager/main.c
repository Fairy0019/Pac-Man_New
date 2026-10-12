#include "Player_Manager.h"
#include <stdio.h>

int main()
{

    // 初始化存档系统
    initSaveSystem();

    // 创建玩家
    PlayerProfile player =
        createPlayer("Player001");

    printf("Nickname:%s\n",
           player.nickname);

    printf("UID:%s\n",
           player.uid);

    // 保存玩家
    savePlayer(&player);

    return 0;
}