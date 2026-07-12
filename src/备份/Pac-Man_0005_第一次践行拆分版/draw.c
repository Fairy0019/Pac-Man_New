#include "draw.h"
//#include "game.h"
#include "texture.h"
#include "pellet.h"

#include "raylib.h"

//==================== 绘制 ====================

void drawGame(void)
{
    ClearBackground(BLACK);

    //====================
    // 绘制地图
    //====================

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            int pixelX = x * TILE_SIZE;
            int pixelY = y * TILE_SIZE + UI_HEIGHT;

            Rectangle dstRect =
            {
                pixelX,
                pixelY,
                TILE_SIZE,
                TILE_SIZE
            };

            // 墙
            if (map[y][x] == '#')
            {
                DrawTexturePro(
                    wallTex,
                    (Rectangle)
                {
                    0,
                        0,
                        TILE_SIZE,
                        TILE_SIZE
                },
                    dstRect,
                    (Vector2) {
                    0, 0
                },
                    0,
                    WHITE
                );
            }

            // 普通豆
            else if (map[y][x] == '.')
            {
                DrawTexturePro(
                    dotTex,
                    (Rectangle)
                {
                    0,
                        0,
                        TILE_SIZE,
                        TILE_SIZE
                },
                    dstRect,
                    (Vector2) {
                    0, 0
                },
                    0,
                    WHITE
                );
            }

            // 能量豆
            else if (map[y][x] == 'o')
            {
                DrawTexturePro(
                    energyDotTex,
                    (Rectangle)
                {
                    0,
                        0,
                        TILE_SIZE,
                        TILE_SIZE
                },
                    dstRect,
                    (Vector2) {
                    0, 0
                },
                    0,
                    WHITE
                );
            }
        }
    }

    //====================
// 绘制吃豆人
//====================

    Rectangle pacmanSrc =
    {
        0,
        0,
        TILE_SIZE,
        TILE_SIZE
    };

    Rectangle pacmanDst =
    {
        player.x * TILE_SIZE + TILE_SIZE / 2,
        player.y * TILE_SIZE + UI_HEIGHT + TILE_SIZE / 2,
        TILE_SIZE,
        TILE_SIZE
    };

    Vector2 pacmanOrigin =
    {
        TILE_SIZE / 2,
        TILE_SIZE / 2
    };

    float rotation = 0;

    // 朝向右
    if (pacmanDirection == 0)
    {
        pacmanSrc.width = TILE_SIZE;
        rotation = 0;
    }

    // 朝向左
    else if (pacmanDirection == 1)
    {
        pacmanSrc.width = -TILE_SIZE;
        rotation = 0;
    }

    // 朝向上
    else if (pacmanDirection == 2)
    {
        pacmanSrc.width = TILE_SIZE;
        rotation = -90;
    }

    // 朝向下
    else if (pacmanDirection == 3)
    {
        pacmanSrc.width = TILE_SIZE;
        rotation = 90;
    }

    DrawTexturePro(
        pacmanTex[currentPacmanFrame],
        pacmanSrc,
        pacmanDst,
        pacmanOrigin,
        rotation,
        WHITE
    );

    //====================
// 绘制幽灵
//====================

    Rectangle ghostDst =
    {
        ghost.x * TILE_SIZE,
        ghost.y * TILE_SIZE + UI_HEIGHT,
        TILE_SIZE,
        TILE_SIZE
    };

    DrawTexturePro(
        ghostTex,
        (Rectangle)
    {
        0,
            0,
            TILE_SIZE,
            TILE_SIZE
    },
        ghostDst,
        (Vector2)
    {
        0,
            0
    },
        0,
        WHITE
    );

    //====================
// UI
//====================

    DrawText(
        TextFormat(
            "Score: %d   Pellets: %d",
            score,
            pellets
        ),
        10,
        10,
        20,
        WHITE
    );

    if (powerMode)
    {
        DrawText(
            "POWER MODE",
            10,
            35,
            20,
            YELLOW
        );
    }

    if (gameOver)
    {
        const char* msg =
            "GAME OVER - Press R to restart";

        int tw = MeasureText(msg, 20);

        DrawText(
            msg,
            (WIDTH * TILE_SIZE - tw) / 2,
            HEIGHT * TILE_SIZE / 2 + UI_HEIGHT,
            20,
            RED
        );
    }

    if (win)
    {
        const char* msg =
            "YOU WIN! - Press R to restart";

        int tw = MeasureText(msg, 20);

        DrawText(
            msg,
            (WIDTH * TILE_SIZE - tw) / 2,
            HEIGHT * TILE_SIZE / 2 + UI_HEIGHT,
            20,
            GREEN
        );
    }
}