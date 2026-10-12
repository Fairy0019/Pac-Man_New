#include <stdio.h>

#include "account.h"
#include "player_data.h"
#include "account_menu.h"
#include "history.h"

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

    if (!History_Init())
    {
        printf("History 初始化失败！\n");

        PlayerData_Shutdown();
        Account_Shutdown();
        return 1;
    }

    // ==================== 当前账户 ====================

    PlayerAccount currentAccount = {0};
    PlayerData currentData = {0};
    GameHistory testRecord = {0};
    GameHistory *records = NULL;

    // ==================== 运行账户菜单 ====================

    AccountMenu_Run(
        &currentAccount,
        &currentData);

    /*
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
    
    */
    // ==================== 测试历史记录 ====================

    if (currentAccount.uid[0] != '\0')
    {
        printf("\n========================================\n");
        printf("             当前账户\n");
        printf("========================================\n");

        printf("UID        : %s\n", currentAccount.uid);
        printf("Name       : %s\n", currentAccount.name);
        printf("High Score : %d\n", currentData.highScore);
        printf("Games      : %d\n", currentData.gamesPlayed);

        // GameHistory testRecord = {0};

        testRecord.mode = 1;
        testRecord.score = 1250;
        testRecord.startTime = time(NULL);
        testRecord.durationSeconds = 180;
        testRecord.result = 1;

        printf("\n========================================\n");
        printf("             历史记录测试\n");
        printf("========================================\n");

        if (History_Add(currentAccount.uid, &testRecord))
        {
            printf("历史记录写入成功！\n");
        }
        else
        {
            printf("历史记录写入失败！\n");
        }

        // GameHistory *records = NULL;

        int count = History_LoadAll(currentAccount.uid, &records);

        if (count >= 0)
        {
            printf("读取到 %d 条历史记录。\n", count);

            for (int i = 0; i < count; i++)
            {
                printf("\n第 %d 条记录\n", i + 1);
                printf("游戏模式：%d\n", records[i].mode);
                printf("最终得分：%d\n", records[i].score);
                printf("开始时间：%lld\n",
                       (long long)records[i].startTime);
                printf("游戏时长：%d 秒\n",
                       records[i].durationSeconds);
                printf("游戏结果：%d\n",
                       records[i].result);
            }

            free(records);
        }
        else
        {
            printf("历史记录读取失败！\n");
        }
    }

    // ==================== 关闭 ====================

    PlayerData_Shutdown();
    Account_Shutdown();

    printf("\n账户系统已关闭。\n");

    return 0;
}