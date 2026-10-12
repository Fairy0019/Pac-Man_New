#ifndef ACCOUNT_MENU_H
#define ACCOUNT_MENU_H

#include <stdbool.h>
#include "account.h"
#include "player_data.h"

bool AccountMenu_Run(PlayerAccount *currentAccount, PlayerData *currentData);
bool AccountMenu_CreateAccount(PlayerAccount *account, PlayerData *data);
bool AccountMenu_SelectAccount(PlayerAccount *account, PlayerData *data);

#endif
