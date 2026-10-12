//#include "game.h"
#include "maze.h"
#include "pellet.h"
#include "ghost.h"
#include "input.h"
#include "collision.h"


#include <stdlib.h>

//==================== 能量豆状态 ====================

int powerMode = 0;      // 是否处于吃鬼模式
int powerTimer = 0;     // 剩余帧数（60FPS下360帧=6秒）
int powerCount = 0;     // 当前地图上能量豆的数量

// 能量豆坐标数组
PowerPellet powerPellets[MAX_POWER_PELLET];

//==================== 游戏数据 ====================

char map[HEIGHT][WIDTH + 1];

int score = 0;
int pellets = 0;

int gameOver = 0;
int win = 0;

Point player = { 1,1 };
Point ghost = { WIDTH - 2,HEIGHT - 2 };

//==================== 动画 ====================

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

float ghostTimer = 0.0f;

//==================== 工具 ====================

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
        animTimer = 0;

        seqIndex++;

        if (seqIndex >= 4)
            seqIndex = 0;

        currentPacmanFrame =
            frameSequence[seqIndex];
    }
}

void initGame(void)
{
    do
    {
        generateRandomMaze();

        generateSpawnPoints();

        improveMaze();

    } while (!ensureConnectivity());

    score = 0;
    pellets = 0;

    powerMode = 0;
    powerTimer = 0;
    powerCount = 0;

    generatePowerPellets();

    gameOver = 0;
    win = 0;

    ghostTimer = 0;

    currentPacmanFrame = 0;
    seqIndex = 0;
    animTimer = 0;

    map[player.y][player.x] = ' ';
    map[ghost.y][ghost.x] = ' ';

    for (int y = 0;y < HEIGHT;y++)
    {
        for (int x = 0;x < WIDTH;x++)
        {
            if (map[y][x] == '.' ||
                map[y][x] == 'o')
            {
                pellets++;
            }
        }
    }
}

void updateGame(float deltaTime)
{
    if (gameOver || win)
        return;

    // 1. 玩家输入 + 移动
    handleInput();

    // 2. 检测是否吃到豆子（普通豆或能量豆）
    checkPellet();

    // 3. 更新能量模式剩余时间
    updatePowerMode();

    // 4. 幽灵移动
    ghostTimer += deltaTime;
    if (ghostTimer >= GHOST_MOVE_DELAY)
    {
        ghostTimer = 0;
        moveGhost();

        // 5. 检测与幽灵碰撞（包含吃鬼模式逻辑）
        checkGhostCollision();
    }

    // 6. 更新动画
    updatePacmanAnimation(deltaTime);
}