#include "collision.h"
//#include "game.h"
#include "pellet.h"

void checkPellet(void)
{
    char cur = map[player.y][player.x];

    //------------------------------------
    // 普通豆
    //------------------------------------

    if (cur == '.')
    {
        map[player.y][player.x] = ' ';

        score += 10;

        pellets--;

        if (pellets == 0)
            win = 1;
    }

    //------------------------------------
    // 能量豆
    //------------------------------------

    else if (cur == 'o')
    {
        map[player.y][player.x] = ' ';

        score += 50;

        pellets--;

        powerMode = 1;

        powerTimer = 360;

        collectPowerPellet(
            player.x,
            player.y
        );

        if (pellets == 0)
            win = 1;
    }
}

void checkGhostCollision(void)
{
    if (player.x != ghost.x)
        return;

    if (player.y != ghost.y)
        return;

    //------------------------------------
    // 吃鬼模式
    //------------------------------------

    if (powerMode)
    {
        ghost.x = WIDTH - 2;
        ghost.y = HEIGHT - 2;

        score += 200;
    }
    else
    {
        gameOver = 1;
    }
}