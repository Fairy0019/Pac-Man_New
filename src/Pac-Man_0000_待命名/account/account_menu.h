#ifndef ACCOUNT_MENU_H
#define ACCOUNT_MENU_H

#include <stdbool.h>

#include "account.h"
#include "player_data.h"

// 显示账户菜单并进行交互
bool AccountMenu_Run(
    PlayerAccount *currentAccount,
    PlayerData *currentData
);

// 创建账户
bool AccountMenu_CreateAccount(
    PlayerAccount *account,
    PlayerData *data
);

// 选择已有账户
bool AccountMenu_SelectAccount(
    PlayerAccount *account,
    PlayerData *data
);

#endif