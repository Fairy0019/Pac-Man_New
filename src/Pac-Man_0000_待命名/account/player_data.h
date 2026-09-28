#ifndef PLAYER_DATA_H
#define PLAYER_DATA_H

#include <stdbool.h>

// 玩家游戏数据
typedef struct
{
    int highScore;   // 最高分
    int gamesPlayed; // 游戏次数

    // 后续可以继续添加：
    // int totalScore;
    // int winCount;
    // int loseCount;
    // ...
} PlayerData;

// 初始化玩家数据系统
bool PlayerData_Init(void);

// 创建一份新的玩家数据，并设置默认值
bool PlayerData_Create(PlayerData *data);

// 保存玩家数据
// uid 用于确定保存到哪个玩家的数据文件
bool PlayerData_Save(const PlayerData *data, const char *uid);

// 读取玩家数据
bool PlayerData_Load(PlayerData *data, const char *uid);

// 删除玩家数据
bool PlayerData_Delete(const char *uid);

// 判断某个 UID 的玩家数据是否存在
bool PlayerData_Exists(const char *uid);

// 关闭玩家数据系统
void PlayerData_Shutdown(void);

#endif