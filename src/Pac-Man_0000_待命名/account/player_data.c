#include "player_data.h"

#include <stdio.h>
#include <string.h>
#include <direct.h>
#include <errno.h>

// ==================== 路径定义 ====================

#define SAVE_DIR "save"
#define PLAYER_DATA_DIR "save/data"

// ==================== 内部函数声明 ====================

// 创建保存目录
static bool PlayerData_CreateDirectory(void);

// 根据 UID 生成玩家数据文件路径
static bool PlayerData_GetFilePath(char *path, size_t size, const char *uid);

// ==================== 初始化 ====================

bool PlayerData_Init(void)
{
    return PlayerData_CreateDirectory();
}

// ==================== 创建默认数据 ====================

bool PlayerData_Create(PlayerData *data)
{
    if (data == NULL)
    {
        return false;
    }

    // 新玩家的默认数据
    data->highScore = 0;
    data->gamesPlayed = 0;

    return true;
}

// ==================== 保存数据 ====================

bool PlayerData_Save(const PlayerData *data, const char *uid)
{
    if (data == NULL || uid == NULL)
    {
        return false;
    }

    char path[256];

    if (!PlayerData_GetFilePath(path, sizeof(path), uid))
    {
        return false;
    }

    FILE *file = fopen(path, "wb");

    if (file == NULL)
    {
        return false;
    }

    size_t written = fwrite(
        data,
        sizeof(PlayerData),
        1,
        file);

    fclose(file);

    return written == 1;
}

// ==================== 读取数据 ====================

bool PlayerData_Load(PlayerData *data, const char *uid)
{
    if (data == NULL || uid == NULL)
    {
        return false;
    }

    char path[256];

    if (!PlayerData_GetFilePath(path, sizeof(path), uid))
    {
        return false;
    }

    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        return false;
    }

    size_t read = fread(
        data,
        sizeof(PlayerData),
        1,
        file);

    fclose(file);

    return read == 1;
}

// ==================== 删除数据 ====================

bool PlayerData_Delete(const char *uid)
{
    if (uid == NULL)
    {
        return false;
    }

    char path[256];

    if (!PlayerData_GetFilePath(path, sizeof(path), uid))
    {
        return false;
    }

    // remove() 成功返回 0
    return remove(path) == 0;
}

// ==================== 判断数据是否存在 ====================

bool PlayerData_Exists(const char *uid)
{
    if (uid == NULL)
    {
        return false;
    }

    char path[256];

    if (!PlayerData_GetFilePath(path, sizeof(path), uid))
    {
        return false;
    }

    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        return false;
    }

    fclose(file);

    return true;
}

// ==================== 关闭 ====================

void PlayerData_Shutdown(void)
{
    // 当前没有需要释放的资源
}

// ==================== 内部函数 ====================

// 创建 save/data 目录
static bool PlayerData_CreateDirectory(void)
{
    // 创建 save
    if (_mkdir(SAVE_DIR) != 0)
    {
        if (errno != EEXIST)
        {
            return false;
        }
    }

    // 创建 save/data
    if (_mkdir(PLAYER_DATA_DIR) != 0)
    {
        if (errno != EEXIST)
        {
            return false;
        }
    }

    return true;
}

// 根据 UID 生成文件路径
static bool PlayerData_GetFilePath(
    char *path,
    size_t size,
    const char *uid)
{
    if (path == NULL || uid == NULL)
    {
        return false;
    }

    int result = snprintf(
        path,
        size,
        "%s/%s.dat",
        PLAYER_DATA_DIR,
        uid);

    // snprintf 返回值 >= size 说明发生了截断
    if (result < 0 || (size_t)result >= size)
    {
        return false;
    }

    return true;
}