#include "ghost.h"
#include "game.h"

#include <stdlib.h>

//==================== 幽灵AI ====================

void moveGhost(void)
{
    Point choices[4];

    int count = 0;

    int bestDist = 100000;

    const Point dirs[4] =
    {
        { 1, 0 },
        {-1, 0 },
        { 0, 1 },
        { 0,-1 }
    };

    //====================
    // 找距离玩家最近的方向
    //====================

    for (int i = 0; i < 4; i++)
    {
        int nx = ghost.x + dirs[i].x;
        int ny = ghost.y + dirs[i].y;

        if (!canMove(nx, ny))
            continue;

        int dist =
            abs(player.x - nx) +
            abs(player.y - ny);

        if (dist < bestDist)
        {
            bestDist = dist;

            count = 0;

            choices[count++] =
                (Point){ nx, ny };
        }
        else if (dist == bestDist)
        {
            choices[count++] =
                (Point){ nx, ny };
        }
    }

    //====================
    // 如果没有最佳方向
    // 随机走一个可走方向
    //====================

    if (count == 0)
    {
        for (int i = 0; i < 4; i++)
        {
            int nx = ghost.x + dirs[i].x;
            int ny = ghost.y + dirs[i].y;

            if (canMove(nx, ny))
            {
                choices[count++] =
                    (Point){ nx, ny };
            }
        }
    }

    //====================
    // 多个最佳方向随机选择
    //====================

    if (count > 0)
    {
        ghost =
            choices[rand() % count];
    }
}