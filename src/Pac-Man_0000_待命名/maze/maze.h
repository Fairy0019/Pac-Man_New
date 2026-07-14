#pragma once

#ifndef MAZE_H
#define MAZE_H

#include "game.h"

//==================== 地图生成 ====================

//生成随机迷宫
void generateRandomMaze(void);

//优化迷宫
void improveMaze(void);

//生成出生点
void generateSpawnPoints(void);

//检查地图是否完全连通
int ensureConnectivity(void);


//==================== 地图工具 ====================

//当前位置可通行方向数量
int countNeighbors(int x, int y);

//当前位置开放方向数量（出生点使用）
int countOpen(int x, int y);

#endif