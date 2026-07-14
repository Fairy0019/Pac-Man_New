#pragma once

#include <time.h>

#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#define MAX_PLAYER 10

typedef struct
{
    char nickname[32];

    char uid[21];

    time_t createTime;

    // int score;     //这应当归游戏本体管

    int highScore;

    int gamesPlayed;

    // int totalScore; // 累计获得分数

    // int totalPellets; // 吃掉的豆子总数

    // int totalGhosts; // 吃掉鬼魂总数

    // int playTime; // 总游玩时间（秒）

    // int achievements[64]; // 成就

} PlayerProfile;

// 初始化
void initPlayerManager();

// 创建玩家
PlayerProfile createPlayer(const char *nickname);

// 获取当前玩家
PlayerProfile *getCurrentPlayer();

// 切换玩家
void switchPlayer(int index);

void initSaveSystem(void);

//保存玩家
void savePlayer(const PlayerProfile *player);

#endif