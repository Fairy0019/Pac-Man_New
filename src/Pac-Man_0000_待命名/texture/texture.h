#pragma once

#ifndef TEXTURE_H
#define TEXTURE_H

#include "raylib.h"

//==================== 图片 ====================

extern Texture2D pacmanTex[3];

extern Texture2D ghostTex;

extern Texture2D wallTex;

extern Texture2D dotTex;

extern Texture2D energyDotTex;

//==================== 接口 ====================

void loadTextures(void);

void unloadTextures(void);

#endif