#include "texture.h"

#include <stdio.h>

//==================== 图片 ====================

Texture2D pacmanTex[3];

Texture2D ghostTex;

Texture2D wallTex;

Texture2D dotTex;

Texture2D energyDotTex;

//==================== 加载 ====================

void loadTextures(void)
{
    pacmanTex[0] =
        LoadTexture("resources/pacman_0.png");

    pacmanTex[1] =
        LoadTexture("resources/pacman_1.png");

    pacmanTex[2] =
        LoadTexture("resources/pacman_2.png");

    ghostTex =
        LoadTexture("resources/ghost_normal.png");

    wallTex =
        LoadTexture("resources/wall.png");

    dotTex =
        LoadTexture("resources/gold_coin.png");

    energyDotTex =
        LoadTexture("resources/energydot.png");


    TraceLog(LOG_INFO,
        "pacman0 = %u",
        pacmanTex[0].id);

    TraceLog(LOG_INFO,
        "pacman1 = %u",
        pacmanTex[1].id);

    TraceLog(LOG_INFO,
        "pacman2 = %u",
        pacmanTex[2].id);

    TraceLog(LOG_INFO,
        "ghost = %u",
        ghostTex.id);

    TraceLog(LOG_INFO,
        "wall = %u",
        wallTex.id);

    TraceLog(LOG_INFO,
        "dot = %u",
        dotTex.id);

    TraceLog(LOG_INFO,
        "energy = %u",
        energyDotTex.id);


    for (int i = 0; i < 3; i++)
    {
        SetTextureFilter(
            pacmanTex[i],
            TEXTURE_FILTER_POINT
        );
    }

    SetTextureFilter(
        ghostTex,
        TEXTURE_FILTER_POINT);

    SetTextureFilter(
        wallTex,
        TEXTURE_FILTER_POINT);

    SetTextureFilter(
        dotTex,
        TEXTURE_FILTER_POINT);

    SetTextureFilter(
        energyDotTex,
        TEXTURE_FILTER_POINT);
}

//==================== 卸载 ====================

void unloadTextures(void)
{
    for (int i = 0; i < 3; i++)
    {
        UnloadTexture(
            pacmanTex[i]);
    }

    UnloadTexture(ghostTex);

    UnloadTexture(wallTex);

    UnloadTexture(dotTex);

    UnloadTexture(energyDotTex);
}