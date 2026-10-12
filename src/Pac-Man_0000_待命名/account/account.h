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
} PlayerAccount;

bool Account_Init(void);
bool Account_Create(PlayerAccount *account, const char *name);
bool Account_Save(const PlayerAccount *account);
bool Account_Load(PlayerAccount *account, const char *uid);
bool Account_Delete(const char *uid);
bool Account_Exists(const char *uid);
int Account_List(PlayerAccount *accounts, int maxAccounts);
void Account_Shutdown(void);

#endif
