#include "raylib.h"
#include "game.h"
#include "draw.h"
#include "texture.h"

#include <stdlib.h>
#include <time.h>

int main(void)
{
    srand((unsigned)time(NULL));

    InitWindow(
        WIDTH * TILE_SIZE,
        HEIGHT * TILE_SIZE + UI_HEIGHT,
        "Pac-Man"
    );

    SetTargetFPS(60);

    //加载所有图片
    loadTextures();

    //初始化游戏
    initGame();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        //ESC退出
        if (IsKeyPressed(KEY_ESCAPE))
            break;

        //Q退出
        if (IsKeyPressed(KEY_Q))
            break;

        //重新开始
        if (IsKeyPressed(KEY_R))
            initGame();

        //游戏更新
        updateGame(deltaTime);

        BeginDrawing();

        drawGame();

        EndDrawing();
    }

    //释放图片
    unloadTextures();

    CloseWindow();

    return 0;
}