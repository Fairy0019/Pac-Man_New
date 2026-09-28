#pragma once
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <stdbool.h>
#include <time.h>

#define ACCOUNT_NAME_MAX 20
#define ACCOUNT_UID_MAX 20
#define ACCOUNT_MAX 16

typedef struct
{
    char uid[ACCOUNT_UID_MAX + 1];
    char name[ACCOUNT_NAME_MAX + 1];

    time_t createTime;

    // int highScore;
    // int gamesPlayed;

} PlayerAccount;

/* 初始化账户系统 */
bool Account_Init(void);

/* 创建一个新账户 */
bool Account_Create(PlayerAccount *account, const char *name);

/* 保存账户 */
bool Account_Save(const PlayerAccount *account);

/* 加载账户 */
bool Account_Load(PlayerAccount *account, const char *uid);

/* 删除账户 */
bool Account_Delete(const char *uid);

/* 判断账户是否存在 */
bool Account_Exists(const char *uid);

int Account_List(
    PlayerAccount *accounts,
    int maxAccounts
);

/* 关闭账户系统 */
void Account_Shutdown(void);

#endif