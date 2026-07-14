#include "Player_Manager.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <direct.h> // _mkdir
#include <errno.h>  // errno

//建立保存系统
static void createDirectory(const char *path)
{
    if (_mkdir(path) == 0)
    {
        printf("[OK] Create: %s\n", path);
    }
    else if (errno == EEXIST)
    {
        printf("[OK] Exists: %s\n", path);
    }
    else
    {
        printf("[ERROR] Failed: %s\n", path);
    }
}

void initSaveSystem(void)
{
    FILE *fp;

    // 创建保存目录
    createDirectory("save");
    createDirectory("save/system");
    createDirectory("save/players");

    // 检查 UID 文件是否存在
    fp = fopen("save/system/uid_counter.dat", "r");

    if (fp == NULL)
    {
        // 第一次运行，创建文件
        fp = fopen("save/system/uid_counter.dat", "w");

        if (fp != NULL)
        {
            fprintf(fp, "%d", 1);
            fclose(fp);

            printf("Create uid_counter.dat\n");
        }
        else
        {
            printf("Create uid_counter.dat Failed!\n");
        }
    }
    else
    {
        fclose(fp);
    }
}

static PlayerProfile players[MAX_PLAYER];

static int playerCount = 0;

static int currentPlayer = -1;

//==================== 内部函数声明 ====================

static void generateUID(char uid[]);

static void getTimeString(char timeStr[]);

static int readUIDCounter(void);

static void saveUIDCounter(int counter);

// //==================== 对外接口 ====================

// initPlayerManager()
// createPlayer()
// getCurrentPlayer()
// switchPlayer()

// //==================== 内部实现 ====================

// generateUID()
// getTimeString()
// readUIDCounter()
// saveUIDCounter()

void initPlayerManager()
{
    playerCount = 0;
    currentPlayer = -1;
}

PlayerProfile createPlayer(const char *nickname)
{
    PlayerProfile player;

    // 昵称
    strcpy(player.nickname, nickname);

    // 创建时间
    player.createTime = time(NULL);

    // 生成UID
    generateUID(player.uid);

    // 初始化数据
    player.highScore = 0;
    player.gamesPlayed = 0;

    return player;
}

PlayerProfile *getCurrentPlayer()
{
    if (currentPlayer < 0)
        return NULL;

    return &players[currentPlayer];
}

static void generateUID(char uid[])
{
    
    char timeStr[15];

    getTimeString(timeStr);

    int counter = readUIDCounter();

    sprintf(uid, "%s%06d", timeStr, counter);

    saveUIDCounter(counter + 1);
}

static void getTimeString(char timeStr[])
{
    time_t now = time(NULL);

    struct tm *t = localtime(&now);

    sprintf(timeStr,
            "%04d%02d%02d%02d%02d%02d",
            t->tm_year + 1900,
            t->tm_mon + 1,
            t->tm_mday,
            t->tm_hour,
            t->tm_min,
            t->tm_sec);
}

static void saveUIDCounter(int counter)
{

}

static int readUIDCounter(void)
{
    return 1;
}

//保存玩家
void savePlayer(const PlayerProfile *player)
{
    char folderPath[128];

    char filePath[160];

    // 创建玩家文件夹路径
    sprintf(folderPath,
            "save/players/%s",
            player->uid);

    // 创建玩家文件夹
    _mkdir(folderPath);

    // 创建 profile.dat 路径
    sprintf(filePath,
            "%s/profile.dat",
            folderPath);

    // 打开文件
    FILE *fp = fopen(filePath, "w");

    if (fp == NULL)
    {
        printf("Save player failed!\n");
        return;
    }

    // 写入玩家数据
    fprintf(fp,
            "Nickname=%s\n",
            player->nickname);

    fprintf(fp,
            "UID=%s\n",
            player->uid);

    fprintf(fp,
            "CreateTime=%lld\n",
            (long long)player->createTime);

    fprintf(fp,
            "HighScore=%d\n",
            player->highScore);

    fprintf(fp,
            "GamesPlayed=%d\n",
            player->gamesPlayed);

    fclose(fp);

    printf("Player saved.\n");
}