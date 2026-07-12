#pragma once

#ifndef PELLET_H
#define PELLET_H

#include "game.h"

//#define MAX_POWER_PELLET 10   // 最多同时存在10颗能量豆
//
//typedef struct
//{
//    int x;
//    int y;
//} PowerPellet;

//==================== 能量豆 ====================

//生成能量豆
void generatePowerPellets(void);

//吃到能量豆检测
void checkPowerPellet(void);

//更新能量模式
void updatePowerMode(void);

//删除一个能量豆
void collectPowerPellet(int x, int y);

#endif