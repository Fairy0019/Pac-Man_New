#pragma once

#ifndef GAME_H
#define GAME_H

#include "raylib.h"

//==================== 地图大小 ====================

#define WIDTH 31
#define HEIGHT 21

#define TILE_SIZE 48
#define UI_HEIGHT 60

#define GHOST_MOVE_DELAY 0.64f

#define MAX_POWER_PELLET 10

typedef struct
{
    int x;
    int y;
} PowerPellet;

//==================== 坐标 ====================

typedef struct
{
    int x;
    int y;
} Point;

//==================== 游戏数据 ====================

extern char map[HEIGHT][WIDTH + 1];

extern Point player;
extern Point ghost;

extern int score;
extern int pellets;

extern int gameOver;
extern int win;

//==================== 能量豆状态 ====================

extern int powerMode;
extern int powerTimer;
extern int powerCount;
extern PowerPellet powerPellets[MAX_POWER_PELLET];

//==================== 动画 ====================

extern int pacmanDirection;
extern int currentPacmanFrame;

extern float animTimer;
extern int seqIndex;

//==================== 幽灵 ====================

extern float ghostTimer;

//==================== 游戏函数 ====================

void initGame(void);

void updateGame(float deltaTime);

void updatePacmanAnimation(float deltaTime);

int canMove(int x, int y);

void generateSpawnPoints(void);

int countOpen(int x, int y);

#endif