#ifndef ACCOUNT_UI_H
#define ACCOUNT_UI_H

#include "account.h"
#include "player_data.h"

typedef enum
{
    ACCOUNT_UI_ACTION_NONE = 0,
    ACCOUNT_UI_ACTION_ENTERED,
    ACCOUNT_UI_ACTION_QUIT
} AccountUIAction;

void AccountUI_Init(void);
AccountUIAction AccountUI_Update(PlayerAccount *account, PlayerData *data);
void AccountUI_Draw(void);

#endif
