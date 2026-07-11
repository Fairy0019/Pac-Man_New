#include "raylib.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>

#define WIDTH 31
#define HEIGHT 21
#define MAZE_W ((WIDTH - 1) / 2)
#define MAZE_H ((HEIGHT - 1) / 2)
#define TILE_SIZE 48
#define UI_HEIGHT 60

//==================== 游戏数据 ====================

// 当前地图（会被修改）
char map[HEIGHT][WIDTH + 1];

int score = 0;
int pellets = 0;
int gameOver = 0;
int win = 0;

typedef struct
{
    int x;
    int y;
} Point;

Point player = { 1, 1 };
Point ghost = { WIDTH - 2, HEIGHT - 2 };

//==================== 图片 ====================

// 三帧吃豆人
Texture2D pacmanTex[3];

// 幽灵
Texture2D ghostTex;

// 墙
Texture2D wallTex;

// 普通豆
Texture2D dotTex;

// 能量豆
Texture2D energyDotTex;

//==================== 动画 ====================

// 吃豆人朝向
// 0 右
// 1 左
// 2 上
// 3 下
int pacmanDirection = 0;

int currentPacmanFrame = 0;

float animTimer = 0.0f;

const float ANIM_INTERVAL = 0.1f;

const int frameSequence[] =
{
    0,
    1,
    2,
    1
};

int seqIndex = 0;

//==================== 幽灵 ====================

#define GHOST_MOVE_DELAY 0.64f

float ghostTimer = 0.0f;

//==================== 工具函数 ====================

static inline int isWall(int x, int y)
{
    return map[y][x] == '#';
}

int canMove(int x, int y)
{
    if (x < 0 || x >= WIDTH)
        return 0;

    if (y < 0 || y >= HEIGHT)
        return 0;

    return !isWall(x, y);
}

void updatePacmanAnimation(float deltaTime)
{
    animTimer += deltaTime;

    if (animTimer >= ANIM_INTERVAL)
    {
        animTimer = 0.0f;

        seqIndex++;

        if (seqIndex >= 4)
            seqIndex = 0;

        currentPacmanFrame = frameSequence[seqIndex];
    }
}

//==================== 随机迷宫 ====================

static void shuffleDirections(int dirs[4][2])
{
    for (int i = 3; i > 0; i--)
    {
        int j = rand() % (i + 1);

        int tx = dirs[i][0];
        int ty = dirs[i][1];

        dirs[i][0] = dirs[j][0];
        dirs[i][1] = dirs[j][1];

        dirs[j][0] = tx;
        dirs[j][1] = ty;
    }
}

void generateRandomMaze(void)
{
    // 全部填成墙
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            map[y][x] = '#';
        }
        map[y][WIDTH] = '\0';
    }

    int visited[MAZE_H][MAZE_W];
    memset(visited, 0, sizeof(visited));

    int stack[WIDTH * HEIGHT][2];
    int top = 0;

    int cx = 0;
    int cy = 0;

    visited[0][0] = 1;
    map[1][1] = '.';

    while (1)
    {
        int dirs[4][2] =
        {
            { 1, 0 },
            { -1, 0 },
            { 0, 1 },
            { 0,-1 }
        };

        shuffleDirections(dirs);

        int moved = 0;

        for (int i = 0; i < 4; i++)
        {
            int nx = cx + dirs[i][0];
            int ny = cy + dirs[i][1];

            if (nx < 0 || nx >= MAZE_W)
                continue;

            if (ny < 0 || ny >= MAZE_H)
                continue;

            if (visited[ny][nx])
                continue;

            // 打通中间墙
            map[1 + 2 * cy + dirs[i][1]]
                [1 + 2 * cx + dirs[i][0]] = '.';

            // 新格子
            map[1 + 2 * ny]
                [1 + 2 * nx] = '.';

            visited[ny][nx] = 1;

            stack[top][0] = cx;
            stack[top][1] = cy;
            top++;

            cx = nx;
            cy = ny;

            moved = 1;
            break;
        }

        // 四个方向都不能走，就回退
        if (!moved)
        {
            if (top == 0)
            {
                break;
            }

            top--;

            cx = stack[top][0];
            cy = stack[top][1];
        }
    }
}

//==================== 初始化 ====================

void initGame(void)
{
    generateRandomMaze();

    score = 0;
    pellets = 0;
    gameOver = 0;
    win = 0;

    player.x = 1;
    player.y = 1;

    ghost.x = WIDTH - 2;
    ghost.y = HEIGHT - 2;

    ghostTimer = 0.0f;

    currentPacmanFrame = 0;
    seqIndex = 0;
    animTimer = 0.0f;

    int powerCount = 0;

    for (int y = 1; y < HEIGHT - 1; y++)
    {
        for (int x = 1; x < WIDTH - 1; x++)
        {
            if (map[y][x] == '#')
                continue;

            if ((x == player.x && y == player.y) ||
                (x == ghost.x && y == ghost.y))
                continue;

            if (powerCount < 4 && rand() % 10 == 0)
            {
                map[y][x] = 'o';
                powerCount++;
            }
        }
    }

    map[player.y][player.x] = ' ';
    map[ghost.y][ghost.x] = ' ';

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (map[y][x] == '.' || map[y][x] == 'o')
                pellets++;
        }
    }
}

//==================== 幽灵AI ====================

void moveGhost(void)
{
    Point choices[4];

    int count = 0;
    int bestDist = 100000;

    const Point dirs[4] =
    {
        {1,0},
        {-1,0},
        {0,1},
        {0,-1}
    };

    for (int i = 0; i < 4; i++)
    {
        int nx = ghost.x + dirs[i].x;
        int ny = ghost.y + dirs[i].y;

        if (!canMove(nx, ny))
            continue;

        int dist = abs(player.x - nx) + abs(player.y - ny);

        if (dist < bestDist)
        {
            bestDist = dist;
            count = 0;
            choices[count++] = (Point){ nx, ny };
        }
        else if (dist == bestDist)
        {
            choices[count++] = (Point){ nx, ny };
        }
    }

    if (count == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            int nx = ghost.x + dirs[i].x;
            int ny = ghost.y + dirs[i].y;

            if (canMove(nx, ny))
            {
                choices[count++] = (Point){ nx, ny };
            }
        }
    }

    if (count > 0)
    {
        ghost = choices[rand() % count];
    }
}

//==================== 绘制 ====================

void drawGame(void)
{
    ClearBackground(BLACK);

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

            if (map[y][x] == '#')
            {
                DrawTexturePro(
                    wallTex,
                    (Rectangle) {
                    0, 0, TILE_SIZE, TILE_SIZE
                },
                    dstRect,
                    (Vector2) {
                    0, 0
                },
                    0,
                    WHITE
                );
            }
            else if (map[y][x] == '.')
            {
                DrawTexturePro(
                    dotTex,
                    (Rectangle) {
                    0, 0, TILE_SIZE, TILE_SIZE
                },
                    dstRect,
                    (Vector2) {
                    0, 0
                },
                    0,
                    WHITE
                );
            }
            else if (map[y][x] == 'o')
            {
                DrawTexturePro(
                    energyDotTex,
                    (Rectangle) {
                    0, 0, TILE_SIZE, TILE_SIZE
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

    //==================== 绘制吃豆人 ====================

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

    if (pacmanDirection == 0) //右
    {
        pacmanSrc.width = TILE_SIZE;
        rotation = 0;
    }

    else if (pacmanDirection == 1) //左
    {
        pacmanSrc.width = -TILE_SIZE;
        rotation = 0;
    }

    else if (pacmanDirection == 2) //上
    {
        pacmanSrc.width = TILE_SIZE;
        rotation = -90;
    }

    else if (pacmanDirection == 3) //下
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

    //==================== 绘制幽灵 ====================

    Rectangle ghostDst =
    {
        ghost.x * TILE_SIZE,
        ghost.y * TILE_SIZE + UI_HEIGHT,
        TILE_SIZE,
        TILE_SIZE
    };

    DrawTexturePro(
        ghostTex,
        (Rectangle) {
        0, 0, TILE_SIZE, TILE_SIZE
    },
        ghostDst,
        (Vector2) {
        0, 0
    },
        0,
        WHITE
    );

    //==================== UI ====================

    DrawText(
        TextFormat("Score: %d   Pellets: %d", score, pellets),
        10,
        10,
        20,
        WHITE
    );

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

//==================== main ====================

int main(void)
{
    TraceLog(LOG_INFO, "%s", GetApplicationDirectory());

    srand((unsigned)time(NULL));

    InitWindow(
        WIDTH * TILE_SIZE,
        HEIGHT * TILE_SIZE + UI_HEIGHT,
        "Pac-Man"
    );

    SetTargetFPS(60);


    pacmanTex[0] = LoadTexture("resources/pacman_0.png");
    pacmanTex[1] = LoadTexture("resources/pacman_1.png");
    pacmanTex[2] = LoadTexture("resources/pacman_2.png");

    ghostTex = LoadTexture("resources/ghost_normal.png");

    wallTex = LoadTexture("resources/wall.png");

    dotTex = LoadTexture("resources/gold_coin.png");

    energyDotTex = LoadTexture("resources/energydot.png");


    TraceLog(LOG_INFO, "pacman0 = %u", pacmanTex[0].id);
    TraceLog(LOG_INFO, "pacman1 = %u", pacmanTex[1].id);
    TraceLog(LOG_INFO, "pacman2 = %u", pacmanTex[2].id);
    TraceLog(LOG_INFO, "ghost    = %u", ghostTex.id);
    TraceLog(LOG_INFO, "wall     = %u", wallTex.id);
    TraceLog(LOG_INFO, "dot      = %u", dotTex.id);
    TraceLog(LOG_INFO, "energy   = %u", energyDotTex.id);

    for (int i = 0;i < 3;i++)
        SetTextureFilter(pacmanTex[i], TEXTURE_FILTER_POINT);

    SetTextureFilter(ghostTex, TEXTURE_FILTER_POINT);
    SetTextureFilter(wallTex, TEXTURE_FILTER_POINT);
    SetTextureFilter(dotTex, TEXTURE_FILTER_POINT);
    SetTextureFilter(energyDotTex, TEXTURE_FILTER_POINT);

    initGame();

    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        if (IsKeyPressed(KEY_ESCAPE))
            break;

        if (IsKeyPressed(KEY_Q))
            break;

        if (IsKeyPressed(KEY_R))
            initGame();

        if (!gameOver && !win)
        {
            int nextX = player.x;
            int nextY = player.y;
            int moved = 0;

            if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP))
            {
                nextY--;
                pacmanDirection = 2;
                moved = 1;
            }

            if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN))
            {
                nextY++;
                pacmanDirection = 3;
                moved = 1;
            }

            if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT))
            {
                nextX--;
                pacmanDirection = 1;
                moved = 1;
            }

            if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT))
            {
                nextX++;
                pacmanDirection = 0;
                moved = 1;
            }

            if (moved && canMove(nextX, nextY))
            {
                player.x = nextX;
                player.y = nextY;

                char cur = map[player.y][player.x];

                if (cur == '.' || cur == 'o')
                {
                    map[player.y][player.x] = ' ';

                    score += (cur == 'o') ? 50 : 10;

                    pellets--;

                    if (pellets == 0)
                        win = 1;
                }
            }

            ghostTimer += deltaTime;

            if (ghostTimer >= GHOST_MOVE_DELAY)
            {
                ghostTimer = 0;

                moveGhost();

                if (player.x == ghost.x &&
                    player.y == ghost.y)
                {
                    gameOver = 1;
                }
            }

            updatePacmanAnimation(deltaTime);
        }

        BeginDrawing();
        drawGame();
        EndDrawing();
    }

    for (int i = 0;i < 3;i++)
        UnloadTexture(pacmanTex[i]);

    UnloadTexture(ghostTex);
    UnloadTexture(wallTex);
    UnloadTexture(dotTex);
    UnloadTexture(energyDotTex);

    CloseWindow();

    return 0;
}