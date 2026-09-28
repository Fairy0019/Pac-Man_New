#include "account.h"
#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <direct.h>
#include <io.h>
#include <stdint.h>

#define SAVE_DIR "save"
#define SYSTEM_DIR "save/system"
#define ACCOUNT_DIR "save/accounts"

#define UID_COUNTER_FILE "save/system/uid_counter.dat"

/* =========================================================
   内部函数
   ========================================================= */

static bool Account_CreateDirectories(void)
{
    if (_access(SAVE_DIR, 0) != 0)
    {
        if (_mkdir(SAVE_DIR) != 0)
            return false;
    }

    if (_access(SYSTEM_DIR, 0) != 0)
    {
        if (_mkdir(SYSTEM_DIR) != 0)
            return false;
    }

    if (_access(ACCOUNT_DIR, 0) != 0)
    {
        if (_mkdir(ACCOUNT_DIR) != 0)
            return false;
    }

    return true;
}

static unsigned int Account_LoadUIDCounter(void)
{
    FILE *file = fopen(
        UID_COUNTER_FILE,
        "rb");

    if (file == NULL)
    {
        return 0;
    }

    unsigned int counter = 0;

    fread(
        &counter,
        sizeof(unsigned int),
        1,
        file);

    fclose(file);

    return counter;
}

static bool Account_SaveUIDCounter(
    unsigned int counter)
{
    FILE *file = fopen(
        UID_COUNTER_FILE,
        "wb");

    if (file == NULL)
        return false;

    size_t written = fwrite(
        &counter,
        sizeof(unsigned int),
        1,
        file);

    fclose(file);

    return written == 1;
}

static bool Account_GenerateUID(
    char *uid,
    size_t size)
{
    unsigned int counter =
        Account_LoadUIDCounter();

    counter++;

    if (!Account_SaveUIDCounter(counter))
        return false;

    snprintf(
        uid,
        size,
        "%08u",
        counter);

    return true;
}

static void Account_GetPath(
    char *path,
    size_t size,
    const char *uid)
{
    snprintf(
        path,
        size,
        "%s/%s.dat",
        ACCOUNT_DIR,
        uid);
}

/* =========================================================
   初始化
   ========================================================= */

bool Account_Init(void)
{
    return Account_CreateDirectories();
}

/* =========================================================
   创建账户
   ========================================================= */

bool Account_Create(PlayerAccount *account, const char *name)
{
    if (account == NULL || name == NULL)
        return false;

    memset(account, 0, sizeof(PlayerAccount));

    strncpy(
        account->name,
        name,
        ACCOUNT_NAME_MAX);

    account->name[ACCOUNT_NAME_MAX] = '\0';

    if(!Account_GenerateUID(
        account->uid,
        sizeof(account->uid)))
    {
        return false;
    }
    
    account->createTime = time(NULL);
    // account->highScore = 0;
    // account->gamesPlayed = 0;

    return Account_Save(account);
}

/* =========================================================
   保存账户
   ========================================================= */

bool Account_Save(const PlayerAccount *account)
{
    if (account == NULL)
        return false;

    char path[256];

    Account_GetPath(
        path,
        sizeof(path),
        account->uid);

    FILE *file = fopen(path, "wb");

    if (file == NULL)
        return false;

    size_t written = fwrite(
        account,
        sizeof(PlayerAccount),
        1,
        file);

    fclose(file);

    return written == 1;
}

/* =========================================================
   加载账户
   ========================================================= */

bool Account_Load(
    PlayerAccount *account,
    const char *uid)
{
    if (account == NULL || uid == NULL)
        return false;

    char path[256];

    Account_GetPath(
        path,
        sizeof(path),
        uid);

    FILE *file = fopen(path, "rb");

    if (file == NULL)
        return false;

    size_t read = fread(
        account,
        sizeof(PlayerAccount),
        1,
        file);

    fclose(file);

    return read == 1;
}

/* =========================================================
   判断账户是否存在
   ========================================================= */

bool Account_Exists(const char *uid)
{
    if (uid == NULL)
        return false;

    char path[256];

    Account_GetPath(
        path,
        sizeof(path),
        uid);

    return _access(path, 0) == 0;
}

/* =========================================================
   删除账户
   ========================================================= */

bool Account_Delete(const char *uid)
{
    if (uid == NULL)
        return false;

    char path[256];

    Account_GetPath(
        path,
        sizeof(path),
        uid);

    return remove(path) == 0;
}

/* =========================================================
   获取账户列表
   ========================================================= */

int Account_List(
    PlayerAccount *accounts,
    int maxAccounts)
{
    if (accounts == NULL || maxAccounts <= 0)
        return 0;

    char searchPath[256];

    snprintf(
        searchPath,
        sizeof(searchPath),
        "%s/*.dat",
        ACCOUNT_DIR
    );

    struct _finddata_t fileInfo;

    intptr_t handle = _findfirst(
        searchPath,
        &fileInfo
    );

    if (handle == -1)
        return 0;

    int count = 0;

    do
    {
        /*
            达到数组上限时停止读取
        */
        if (count >= maxAccounts)
            break;

        /*
            从文件名中提取 UID（去掉 .dat 后缀）
        */

        char uid[ACCOUNT_UID_MAX + 1];
        strncpy(
            uid,
            fileInfo.name,
            ACCOUNT_UID_MAX
        );

        uid[ACCOUNT_UID_MAX] = '\0';

        char* extension = strrchr(uid, '.');
        if (extension != NULL)
        {
            *extension = '\0';
        }

        /*
            加载账户数据
        */    

        if (Account_Load(&accounts[count], uid))
        {
            count++;
        }
        

    } while (_findnext(handle, &fileInfo) == 0);

    _findclose(handle);

    return count;

}


/* =========================================================
   关闭账户系统
   ========================================================= */

void Account_Shutdown(void)
{
}

/* =========================================================
   测试程序
   ========================================================= */

