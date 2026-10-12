#include "history.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <direct.h>
#include <sys/stat.h>

// 历史记录存储目录
#define HISTORY_DIR "save/history"

// 根据 UID 生成历史记录文件路径
static int History_GetFilePath(const char *uid, char *path, size_t pathSize)
{
    size_t i;

    if (uid == NULL || uid[0] == '\0' || path == NULL)
        return 0;

    // UID 只允许使用字母和数字，防止路径注入
    for (i = 0; uid[i] != '\0'; i++)
    {
        if (!((uid[i] >= '0' && uid[i] <= '9') ||
              (uid[i] >= 'a' && uid[i] <= 'z') ||
              (uid[i] >= 'A' && uid[i] <= 'Z')))
        {
            return 0;
        }
    }

    if (snprintf(path, pathSize, "%s/%s.dat", HISTORY_DIR, uid) >= (int)pathSize)
        return 0;

    return 1;
}

// 检查历史记录目录是否存在
static int History_EnsureDirectory(void)
{
    struct stat info;

    if (stat("save", &info) != 0)
    {
        if (_mkdir("save") != 0 && errno != EEXIST)
            return 0;
    }
    else if (!(info.st_mode & S_IFDIR))
    {
        return 0;
    }

    if (stat(HISTORY_DIR, &info) != 0)
    {
        if (_mkdir(HISTORY_DIR) != 0 && errno != EEXIST)
            return 0;
    }
    else if (!(info.st_mode & S_IFDIR))
    {
        return 0;
    }

    return 1;
}

// 初始化历史记录系统
int History_Init(void)
{
    return History_EnsureDirectory();
}

// 关闭历史记录系统
// 当前版本不长期持有文件句柄，因此无需额外清理
void History_Shutdown(void)
{
}

// 添加一条历史记录
int History_Add(const char *uid, const GameHistory *record)
{
    char path[256];
    FILE *file;

    if (record == NULL)
        return 0;

    if (!History_GetFilePath(uid, path, sizeof(path)))
        return 0;

    if (!History_EnsureDirectory())
        return 0;

    file = fopen(path, "ab");

    if (file == NULL)
        return 0;

    if (fwrite(record, sizeof(GameHistory), 1, file) != 1)
    {
        fclose(file);
        return 0;
    }

    if (fclose(file) != 0)
        return 0;

    return 1;
}

// 读取某个账户的全部历史记录
// 成功返回记录数量；失败返回 -1
// 没有历史记录时返回 0，records 为 NULL
int History_LoadAll(const char *uid, GameHistory **records)
{
    char path[256];
    FILE *file;
    long fileSize;
    size_t count;
    GameHistory *buffer;

    if (records == NULL)
        return -1;

    *records = NULL;

    if (!History_GetFilePath(uid, path, sizeof(path)))
        return -1;

    file = fopen(path, "rb");

    // 文件不存在表示该账户尚无历史记录
    if (file == NULL)
    {
        if (errno == ENOENT)
            return 0;

        return -1;
    }

    if (fseek(file, 0, SEEK_END) != 0)
    {
        fclose(file);
        return -1;
    }

    fileSize = ftell(file);

    if (fileSize < 0)
    {
        fclose(file);
        return -1;
    }

    // 文件大小必须是完整记录的整数倍
    if (fileSize % (long)sizeof(GameHistory) != 0)
    {
        fclose(file);
        return -1;
    }

    count = (size_t)(fileSize / (long)sizeof(GameHistory));

    if (count == 0)
    {
        fclose(file);
        return 0;
    }

    if (count > (size_t)-1 / sizeof(GameHistory))
    {
        fclose(file);
        return -1;
    }

    buffer = (GameHistory *)malloc(count * sizeof(GameHistory));

    if (buffer == NULL)
    {
        fclose(file);
        return -1;
    }

    if (fseek(file, 0, SEEK_SET) != 0)
    {
        free(buffer);
        fclose(file);
        return -1;
    }

    if (fread(buffer, sizeof(GameHistory), count, file) != count)
    {
        free(buffer);
        fclose(file);
        return -1;
    }

    if (fclose(file) != 0)
    {
        free(buffer);
        return -1;
    }

    *records = buffer;

    return (int)count;
}