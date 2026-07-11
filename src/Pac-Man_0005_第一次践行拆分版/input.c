#include "raylib.h"
#include "game.h"
#include "input.h"

void handleInput(void)
{
    int nextX = player.x;
    int nextY = player.y;

    int moved = 0;

    //---------------------------------
    // 上
    //---------------------------------

    if (IsKeyPressed(KEY_W) ||
        IsKeyPressed(KEY_UP))
    {
        nextY--;

        pacmanDirection = 2;

        moved = 1;
    }

    //---------------------------------
    // 下
    //---------------------------------

    if (IsKeyPressed(KEY_S) ||
        IsKeyPressed(KEY_DOWN))
    {
        nextY++;

        pacmanDirection = 3;

        moved = 1;
    }

    //---------------------------------
    // 左
    //---------------------------------

    if (IsKeyPressed(KEY_A) ||
        IsKeyPressed(KEY_LEFT))
    {
        nextX--;

        pacmanDirection = 1;

        moved = 1;
    }

    //---------------------------------
    // 右
    //---------------------------------

    if (IsKeyPressed(KEY_D) ||
        IsKeyPressed(KEY_RIGHT))
    {
        nextX++;

        pacmanDirection = 0;

        moved = 1;
    }

    //---------------------------------
    // 真正移动
    //---------------------------------

    if (moved && canMove(nextX, nextY))
    {
        player.x = nextX;
        player.y = nextY;
    }
}