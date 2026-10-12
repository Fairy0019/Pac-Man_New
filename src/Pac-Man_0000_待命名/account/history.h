#ifndef HISTORY_H
#define HISTORY_H

#include <time.h>

// 单局游戏历史记录
typedef struct
{
    int mode;            // 游戏模式
    int score;           // 最终得分
    time_t startTime;    // 开始时间
    int durationSeconds; // 游戏时长（秒）
    int result;          // 游戏结果
} GameHistory;

// 历史记录系统初始化与关闭
int History_Init(void);
void History_Shutdown(void);

// 添加一条历史记录
int History_Add(const char *uid, const GameHistory *record);

// 读取某个账户的全部历史记录
// 成功返回记录数量，失败返回 -1
// 调用者负责释放返回的数组
int History_LoadAll(const char *uid, GameHistory **records);

#endif