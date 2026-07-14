#include "maze.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define MAZE_W ((WIDTH - 1) / 2)
#define MAZE_H ((HEIGHT - 1) / 2)

//==================== 内部函数声明 ====================

static void shuffleDirections(int dirs[4][2]);
static int createsRoom(int x, int y);
static int openNearbyWall(int x, int y);
static void fixDeadEnds(void);
static void floodFill(int x, int y, int visited[HEIGHT][WIDTH]);
static void addStrategicLoopsBySpawn(void);

//==================== 随机方向 ====================

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

//==================== DFS迷宫 ====================

void generateRandomMaze(void)
{
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
            { 1,0},
            {-1,0},
            {0,1},
            {0,-1}
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

            map[1 + 2 * cy + dirs[i][1]]
                [1 + 2 * cx + dirs[i][0]] = '.';

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

        if (!moved)
        {
            if (top == 0)
                break;

            top--;

            cx = stack[top][0];
            cy = stack[top][1];
        }
    }
}

//==================== 增加环路 ====================

static void addStrategicLoopsBySpawn(void)
{
    for (int y = 1; y < HEIGHT - 1; y++)
    {
        for (int x = 1; x < WIDTH - 1; x++)
        {
            if (map[y][x] != '#')
                continue;

            int openVertical =
                map[y - 1][x] != '#' &&
                map[y + 1][x] != '#';

            int openHorizontal =
                map[y][x - 1] != '#' &&
                map[y][x + 1] != '#';

            if (!openVertical && !openHorizontal)
                continue;

            int dp =
                abs(player.x - x) +
                abs(player.y - y);

            int dg =
                abs(ghost.x - x) +
                abs(ghost.y - y);

            int value = 20 - (dp + dg) / 3;

            if (value < 3)
                value = 3;

            if (value > 15)
                value = 15;

            if (rand() % 100 < value)
            {
                map[y][x] = '.';
            }
        }
    }
}

//==================== 工具 ====================

int countNeighbors(int x, int y)
{
    int count = 0;

    if (map[y - 1][x] != '#') count++;
    if (map[y + 1][x] != '#') count++;
    if (map[y][x - 1] != '#') count++;
    if (map[y][x + 1] != '#') count++;

    return count;
}

int countOpen(int x, int y)
{
    return countNeighbors(x, y);
}

static int createsRoom(int x, int y)
{
    int open = 0;

    for (int dy = -1; dy <= 1; dy++)
    {
        for (int dx = -1; dx <= 1; dx++)
        {
            if (dx == 0 && dy == 0)
                continue;

            if (map[y + dy][x + dx] != '#')
                open++;
        }
    }

    return open >= 6;
}

//==================== 打通附近墙 ====================

static int openNearbyWall(int x, int y)
{
    int dirs[4][2] =
    {
        { 1,0 },
        {-1,0 },
        { 0,1 },
        { 0,-1 }
    };

    // 打乱方向
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

    for (int i = 0; i < 4; i++)
    {
        int wx = x + dirs[i][0];
        int wy = y + dirs[i][1];

        if (wx <= 0 || wx >= WIDTH - 1 ||
            wy <= 0 || wy >= HEIGHT - 1)
            continue;

        if (map[wy][wx] != '#')
            continue;

        int bx = x + dirs[i][0] * 2;
        int by = y + dirs[i][1] * 2;

        if (bx <= 0 || bx >= WIDTH - 1 ||
            by <= 0 || by >= HEIGHT - 1)
            continue;

        if (map[by][bx] != '#')
        {
            map[wy][wx] = '.';

            if (createsRoom(wx, wy))
            {
                map[wy][wx] = '#';
            }
            else
            {
                return 1;
            }
        }
    }

    return 0;
}

//==================== 修复死路 ====================

static void fixDeadEnds(void)
{
    int changed;

    do
    {
        changed = 0;

        for (int y = 1; y < HEIGHT - 1; y++)
        {
            for (int x = 1; x < WIDTH - 1; x++)
            {
                if (map[y][x] != '.')
                    continue;

                if (countNeighbors(x, y) == 1)
                {
                    if (openNearbyWall(x, y))
                        changed = 1;
                }
            }
        }

    } while (changed);
}

//==================== FloodFill ====================

static void floodFill(
    int x,
    int y,
    int visited[HEIGHT][WIDTH]
)
{
    if (x <= 0 || x >= WIDTH - 1 ||
        y <= 0 || y >= HEIGHT - 1)
        return;

    if (visited[y][x])
        return;

    if (map[y][x] == '#')
        return;

    visited[y][x] = 1;

    floodFill(x + 1, y, visited);
    floodFill(x - 1, y, visited);
    floodFill(x, y + 1, visited);
    floodFill(x, y - 1, visited);
}

//==================== 连通性检查 ====================

int ensureConnectivity(void)
{
    int visited[HEIGHT][WIDTH] = { 0 };

    floodFill(player.x, player.y, visited);

    int road = 0;
    int reach = 0;

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (map[y][x] != '#')
                road++;

            if (visited[y][x])
                reach++;
        }
    }

    return road == reach;
}

//==================== 优化地图 ====================

void improveMaze(void)
{
    addStrategicLoopsBySpawn();

    fixDeadEnds();

    if (!ensureConnectivity())
    {
        printf("地图不连通\n");
    }
}

//==================== 出生点 ====================

void generateSpawnPoints(void)
{
    // 玩家出生
    while (1)
    {
        int x = rand() % (WIDTH - 2) + 1;
        int y = rand() % (HEIGHT - 2) + 1;

        if (map[y][x] == '#')
            continue;

        if (countOpen(x, y) < 2)
            continue;

        player.x = x;
        player.y = y;
        break;
    }

    // 幽灵出生
    while (1)
    {
        int x = rand() % (WIDTH - 2) + 1;
        int y = rand() % (HEIGHT - 2) + 1;

        if (map[y][x] == '#')
            continue;

        if (countOpen(x, y) < 2)
            continue;

        int dist =
            abs(player.x - x) +
            abs(player.y - y);

        if (dist < 10)
            continue;

        ghost.x = x;
        ghost.y = y;
        break;
    }
}