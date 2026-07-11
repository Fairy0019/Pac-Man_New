#include "pellet.h"
#include "game.h"
#include "maze.h"      // ← 新增

#include <stdlib.h>

#define REGION_X 2
#define REGION_Y 2

//==================== 内部结构 ====================

typedef struct
{
    int x;
    int y;

    int score;
    int valid;

} PowerCandidate;

//==================== 内部函数声明 ====================

static int findBestCandidate(
    PowerCandidate candidates[],
    int count
);

static void suppressNearbyCandidates(
    PowerCandidate candidates[],
    int count,
    int x,
    int y
);

static int getWallDistance(
    int x,
    int y
);

//==================== 找最高分 ====================

static int findBestCandidate(
    PowerCandidate candidates[],
    int count
)
{
    int best = -1;
    int bestScore = -999999;

    for (int i = 0; i < count; i++)
    {
        if (!candidates[i].valid)
            continue;

        if (candidates[i].score > bestScore)
        {
            bestScore = candidates[i].score;
            best = i;
        }
    }

    return best;
}

//==================== 附近降权 ====================

static void suppressNearbyCandidates(
    PowerCandidate candidates[],
    int count,
    int x,
    int y
)
{
    for (int i = 0; i < count; i++)
    {
        if (!candidates[i].valid)
            continue;

        int dist =
            abs(candidates[i].x - x) +
            abs(candidates[i].y - y);

        if (dist < 7)
        {
            candidates[i].score -=
                (7 - dist) * 30;
        }

        if (dist == 0)
        {
            candidates[i].valid = 0;
        }
    }
}

//==================== 距离墙 ====================

static int getWallDistance(int x, int y)
{
    int minDist = 99;

    for (int r = 1; r <= 5; r++)
    {
        for (int dy = -r; dy <= r; dy++)
        {
            for (int dx = -r; dx <= r; dx++)
            {
                int nx = x + dx;
                int ny = y + dy;

                if (map[ny][nx] == '#')
                {
                    minDist = r;
                    return minDist;
                }
            }
        }
    }

    return minDist;
}

//==================== 删除能量豆 ====================

void collectPowerPellet(int x, int y)
{
    for (int i = 0; i < powerCount; i++)
    {
        if (powerPellets[i].x == x &&
            powerPellets[i].y == y)
        {
            powerPellets[i] =
                powerPellets[powerCount - 1];

            powerCount--;

            break;
        }
    }
}

//==================== 检测吃到能量豆 ====================

void checkPowerPellet(void)
{
    for (int i = 0; i < powerCount; i++)
    {
        if (player.x == powerPellets[i].x &&
            player.y == powerPellets[i].y)
        {
            powerMode = 1;

            // 6 秒
            powerTimer = 360;

            collectPowerPellet(
                player.x,
                player.y
            );

            score += 50;

            break;
        }
    }
}

//==================== 能量状态 ====================

void updatePowerMode(void)
{
    if (!powerMode)
        return;

    powerTimer--;

    if (powerTimer <= 0)
    {
        powerMode = 0;
        powerTimer = 0;
    }
}

//==================== 生成能量豆 ====================

void generatePowerPellets(void)
{
    PowerCandidate candidates[WIDTH * HEIGHT];

    int count = 0;

    for (int y = 1; y < HEIGHT - 1; y++)
    {
        for (int x = 1; x < WIDTH - 1; x++)
        {
            if (map[y][x] != '.')
                continue;

            if (x == player.x &&
                y == player.y)
                continue;

            if (x == ghost.x &&
                y == ghost.y)
                continue;

            int score = 0;

            //-----------------------------
            // 距离玩家、幽灵
            //-----------------------------

            int dp =
                abs(player.x - x) +
                abs(player.y - y);

            int dg =
                abs(ghost.x - x) +
                abs(ghost.y - y);

            score += dp;
            score += dg / 2;

            if (score < 1)
                score = 1;

            //-----------------------------
            // 离边缘越远越好
            //-----------------------------

            int edgeDistance = x;

            if (WIDTH - 1 - x < edgeDistance)
                edgeDistance = WIDTH - 1 - x;

            if (y < edgeDistance)
                edgeDistance = y;

            if (HEIGHT - 1 - y < edgeDistance)
                edgeDistance = HEIGHT - 1 - y;

            score += edgeDistance * 8;

            //-----------------------------
            // 路口奖励
            //-----------------------------

            int neighbors =
                countNeighbors(x, y);

            if (neighbors == 1)
                score += 5;

            if (neighbors >= 3)
                score += 8;

            //-----------------------------
            // 随机扰动
            //-----------------------------

            score += rand() % 30;

            candidates[count].x = x;
            candidates[count].y = y;
            candidates[count].score = score;
            candidates[count].valid = 1;

            count++;
        }
    }

    //-----------------------------
    // 四区域统计
    //-----------------------------

    int regionCount[REGION_Y][REGION_X] = { 0 };

    for (int i = 0; i < count; i++)
    {
        int rx =
            candidates[i].x /
            (WIDTH / REGION_X);

        int ry =
            candidates[i].y /
            (HEIGHT / REGION_Y);

        if (rx >= REGION_X)
            rx = REGION_X - 1;

        if (ry >= REGION_Y)
            ry = REGION_Y - 1;

        regionCount[ry][rx]++;
    }

    //-----------------------------
    // 稀疏区域加分
    //-----------------------------

    for (int i = 0; i < count; i++)
    {
        int rx =
            candidates[i].x /
            (WIDTH / REGION_X);

        int ry =
            candidates[i].y /
            (HEIGHT / REGION_Y);

        if (rx >= REGION_X)
            rx = REGION_X - 1;

        if (ry >= REGION_Y)
            ry = REGION_Y - 1;

        if (regionCount[ry][rx] < 30)
        {
            candidates[i].score += 40;
        }
    }

    //-----------------------------
    // 放置能量豆
    //-----------------------------

    int pelletCount = 0;

    while (pelletCount < 7)
    {
        int best =
            findBestCandidate(
                candidates,
                count
            );

        if (best == -1)
            break;

        int x = candidates[best].x;
        int y = candidates[best].y;

        // 地图标记
        map[y][x] = 'o';

        // 保存坐标
        if (powerCount < MAX_POWER_PELLET)
        {
            powerPellets[powerCount].x = x;
            powerPellets[powerCount].y = y;
            powerCount++;
        }

        pelletCount++;

        // 附近位置降权，避免扎堆
        suppressNearbyCandidates(
            candidates,
            count,
            x,
            y
        );
    }
}