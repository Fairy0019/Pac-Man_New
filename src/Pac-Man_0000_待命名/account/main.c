#include <stdio.h>

#include "account.h"
#include "player_data.h"
#include "account_menu.h"

int main(void)
{
    printf("========================================\n");
    printf("       PAC-MAN ACCOUNT SYSTEM TEST\n");
    printf("========================================\n\n");

    // ==================== 初始化 ====================

    if (!Account_Init())
    {
        printf("Account 初始化失败！\n");
        return 1;
    }

    if (!PlayerData_Init())
    {
        printf("PlayerData 初始化失败！\n");

        Account_Shutdown();
        return 1;
    }

    // ==================== 当前账户 ====================

    PlayerAccount currentAccount = {0};
    PlayerData currentData = {0};

    // ==================== 运行账户菜单 ====================

    AccountMenu_Run(
        &currentAccount,
        &currentData);

    // ==================== 测试当前账户 ====================

    if (currentAccount.uid[0] != '\0')
    {
        printf("\n========================================\n");
        printf("             当前账户\n");
        printf("========================================\n");

        printf("UID        : %s\n", currentAccount.uid);
        printf("Name       : %s\n", currentAccount.name);
        printf("High Score : %d\n", currentData.highScore);
        printf("Games      : %d\n", currentData.gamesPlayed);
    }

    // ==================== 关闭 ====================

    PlayerData_Shutdown();
    Account_Shutdown();

    printf("\n账户系统已关闭。\n");

    return 0;
}