#include "account_menu.h"

#include <stdio.h>
#include <string.h>

#include "player_data.h"

// ==================== 内部函数 ====================

// 清理输入缓冲区
static void AccountMenu_ClearInput(void)
{
    int c;

    while ((c = getchar()) != '\n' && c != EOF)
    {
    }
}

// 等待用户按回车
static void AccountMenu_Pause(void)
{
    printf("\n按回车继续...");
    getchar();
}

// 显示账户信息
static void AccountMenu_PrintAccount(const PlayerAccount *account)
{
    if (account == NULL)
    {
        return;
    }

    printf("\n----------------------------------------\n");
    printf("UID        : %s\n", account->uid);
    printf("Name       : %s\n", account->name);
    printf("CreateTime : %lld\n", (long long)account->createTime);
    printf("----------------------------------------\n");
}

// ==================== 创建账户 ====================

bool AccountMenu_CreateAccount(
    PlayerAccount *account,
    PlayerData *data)
{
    if (account == NULL || data == NULL)
    {
        return false;
    }

    // ==================== 输入名称 ====================

    char name[ACCOUNT_NAME_MAX + 1];

    printf("\n========================================\n");
    printf("              创建账户\n");
    printf("========================================\n");

    printf("请输入玩家名称：");

    if (fgets(name, sizeof(name), stdin) == NULL)
    {
        return false;
    }

    // 去掉换行符
    name[strcspn(name, "\n")] = '\0';

    // 检查空名称
    if (name[0] == '\0')
    {
        printf("玩家名称不能为空！\n");
        return false;
    }

    // ==================== 创建账户 ====================

    if (!Account_Create(account, name))
    {
        printf("账户创建失败！\n");
        return false;
    }

    // ==================== 创建玩家数据 ====================

    if (!PlayerData_Create(data))
    {
        printf("玩家数据创建失败！\n");

        return false;
    }

    // ==================== 保存玩家数据 ====================

    if (!PlayerData_Save(data, account->uid))
    {
        printf("玩家数据保存失败！\n");

        return false;
    }

    // ==================== 创建完成 ====================

    printf("\n账户创建成功！\n");

    AccountMenu_PrintAccount(account);

    printf("玩家数据已经初始化。\n");

    return true;
}

// ==================== 确认进入账户 ====================

static bool AccountMenu_EnterAccount(
    const PlayerAccount *account,
    const PlayerData *data,

    PlayerAccount *currentAccount,
    PlayerData *currentData
)

{
    if (account == NULL || data == NULL)
    {
        return false;
    }

    printf("\n========================================\n");
    printf("              账户已选择\n");
    printf("========================================\n");

    printf("玩家：%s\n", account->name);
    printf("UID ：%s\n", account->uid);

    printf("\n");
    printf("1. 进入\n");
    printf("0. 返回\n");

    printf("========================================\n");
    printf("请选择：");

    int choice;

    if (scanf_s("%d", &choice) != 1)
    {
        AccountMenu_ClearInput();

        printf("输入无效！\n");

        return false;
    }

    AccountMenu_ClearInput();

    if (choice == 1)
    {
        *currentAccount = *account;
        *currentData = *data;

        printf("\n账户进入成功！\n");
        printf("等待进入下一个游戏状态……\n");

        return true;
    }

    if (choice == 0)
    {
        return false;
    }

    printf("无效选项！\n");

    return false;
}

static int AccountMenu_AccountAction(
    PlayerAccount *account,
    PlayerData *data)
{
    if (account == NULL || data == NULL)
    {
        return 0;
    }

    while (true)
    {
        printf("\n========================================\n");
        printf("              账户操作\n");
        printf("========================================\n");

        printf("玩家：%s\n", account->name);
        printf("UID ：%s\n", account->uid);

        printf("\n");
        printf("1. 进入游戏\n");
        printf("2. 修改名称\n");
        printf("3. 删除账户\n");
        printf("0. 返回\n");

        printf("========================================\n");
        printf("请选择：");

        int choice;

        if (scanf_s("%d", &choice) != 1)
        {
            AccountMenu_ClearInput();
            printf("输入无效！\n");
            continue;
        }

        AccountMenu_ClearInput();

        if (choice == 1)
        {
            return 1;
        }

        // if (choice == 2)
        // {
        //     printf("\n修改名称功能暂未实现。\n");
        //     continue;
        // }

        if (choice == 2)
        {
            char newName[ACCOUNT_NAME_MAX + 1];

            printf("\n========================================\n");
            printf("              修改名称\n");
            printf("========================================\n");

            printf("当前名称：%s\n", account->name);
            printf("请输入新名称：");

            if (fgets(newName, sizeof(newName), stdin) == NULL)
            {
                printf("输入失败！\n");
                continue;
            }

            // 去掉换行符
            newName[strcspn(newName, "\n")] = '\0';

            // 检查空名称
            if (newName[0] == '\0')
            {
                printf("玩家名称不能为空！\n");
                continue;
            }

            // 修改名称
            strcpy_s(account->name, sizeof(account->name), newName);

            // 保存账户
            if (!Account_Save(account))
            {
                printf("账户保存失败！\n");
                continue;
            }

            printf("\n名称修改成功！\n");
            printf("当前名称：%s\n", account->name);

            continue;
        }

        // if (choice == 3)
        // {
        //     printf("\n删除账户功能暂未实现。\n");
        //     continue;
        // }
        if (choice == 3)
        {
            printf("\n========================================\n");
            printf("              删除账户\n");
            printf("========================================\n");

            printf("玩家：%s\n", account->name);
            printf("UID ：%s\n", account->uid);

            printf("\n警告：此操作将删除账户及其所有游戏数据！\n");
            printf("删除后无法恢复。\n");

            printf("\n1. 确认删除\n");
            printf("0. 返回\n");

            printf("========================================\n");
            printf("请选择：");

            int deleteChoice;

            if (scanf_s("%d", &deleteChoice) != 1)
            {
                AccountMenu_ClearInput();
                printf("输入无效！\n");
                continue;
            }

            AccountMenu_ClearInput();

            if (deleteChoice == 0)
            {
                continue;
            }

            if (deleteChoice != 1)
            {
                printf("无效选项！\n");
                continue;
            }

            // ==================== 删除账户 ====================

            if (!Account_Delete(account->uid))
            {
                printf("\n账户删除失败！\n");
                continue;
            }

            // ==================== 删除玩家数据 ====================

            if (!PlayerData_Delete(account->uid))
            {
                printf("\n玩家数据删除失败！\n");
                continue;
            }

            printf("\n账户删除成功！\n");

            return -1;
        }

        if (choice == 0)
        {
            return 0;
        }

        printf("无效选项！\n");
    }
}

// ==================== 选择账户 ====================

bool AccountMenu_SelectAccount(
    PlayerAccount *account,
    PlayerData *data)
{
    if (account == NULL || data == NULL)
    {
        return false;
    }

    // ==================================================
    // 这里才是“选择账户”自己的循环
    // ==================================================

    while (true)
    {
        // ==================== 获取账户列表 ====================

        PlayerAccount accounts[ACCOUNT_MAX];

        int count = Account_List(
            accounts,
            ACCOUNT_MAX);

        // ==================== 显示账户列表 ====================

        printf("\n========================================\n");
        printf("              选择账户\n");
        printf("========================================\n");

        if (count > 0)
        {
            // 显示已有账户
            for (int i = 0; i < count; i++)
            {
                printf(
                    "%d. %s  [%s]\n",
                    i + 1,
                    accounts[i].name,
                    accounts[i].uid);
            }

            printf("----------------------------------------\n");
        }
        else
        {
            printf("\n        当前没有任何账户。\n\n");
            printf("----------------------------------------\n");
        }

        // ==================== 显示固定选项 ====================

        int createOption = count + 1;

        printf("%d. 新建账户\n", createOption);
        printf("0. 返回\n");

        printf("========================================\n");
        printf("请选择：");

        // ==================== 获取输入 ====================

        int choice;

        if (scanf_s("%d", &choice) != 1)
        {
            AccountMenu_ClearInput();

            printf("输入无效！\n");

            continue;
        }

        AccountMenu_ClearInput();

        // ==================== 返回 ====================

        if (choice == 0)
        {
            return false;
        }

        // ==================== 新建账户 ====================

        if (choice == createOption)
        {
            PlayerAccount newAccount;
            PlayerData newData;

            if (AccountMenu_CreateAccount(
                    &newAccount,
                    &newData))
            {
                // 创建成功以后，
                // 自动把新账户作为当前账户。

                // if ( (
                //         &selectedAccount,
                //         &selectedData,
                //         account,
                //         data
                // ))
                // {
                //     // 用户选择“进入”
                //     return true;
                // }

                // 用户选择“返回”
                // 回到账户列表

                int action = AccountMenu_AccountAction(
                    &newAccount,
                    &newData);

                if (action == 1)
                {
                    *account = newAccount;
                    *data = newData;

                    return true;
                }

                if (action == -1)
                {
                    continue;
                }

                // action == 0
                // 用户返回，继续账户列表
            }

            // 创建失败
            // 或者创建后选择返回
            // 都继续显示账户列表

            continue;
        }

        // ==================== 选择已有账户 ====================

        if (choice < 1 || choice > count)
        {
            printf("无效的账户编号！\n");

            continue;
        }

        // ==================== 读取账户 ====================

        PlayerAccount selectedAccount =
            accounts[choice - 1];

        PlayerData selectedData;

        // 读取玩家数据
        if (!PlayerData_Load(
                &selectedData,
                selectedAccount.uid))
        {
            printf("\n玩家数据读取失败！\n");
            printf("UID：%s\n", selectedAccount.uid);

            continue;
        }

        // ==================== 账户操作 ====================

        int action = AccountMenu_AccountAction(
            &selectedAccount,
            &selectedData);

        if (action == 1)
        {
            *account = selectedAccount;
            *data = selectedData;

            return true;
        }

        if (action == -1)
        {
            continue;
        }

        // action == 0
        // 用户返回，继续账户列表

        // ==================== 确认是否进入 ====================

        // if (AccountMenu_EnterAccount(
        //         account,
        //         data))
        // {
        //     return true;
        // }

        // 如果返回，就继续账户列表
    }
}
// ==================== 主菜单 ====================

bool AccountMenu_Run(
    PlayerAccount *currentAccount,
    PlayerData *currentData)
{
    if (currentAccount == NULL || currentData == NULL)
    {
        return false;
    }

    bool running = true;

    PlayerAccount newAccount;
    PlayerData newData;

    while (running)
    {
        printf("\n");
        printf("========================================\n");
        printf("             PAC-MAN ACCOUNT\n");
        printf("========================================\n");
        printf("1. 创建账户\n");
        printf("2. 选择账户\n");
        printf("3. 退出\n");
        printf("========================================\n");
        printf("请选择：");

        int choice;

        if (scanf_s("%d", &choice) != 1)
        {
            AccountMenu_ClearInput();

            printf("输入无效！\n");

            continue;
        }

        AccountMenu_ClearInput();

        switch (choice)
        {
        case 1:

            if (AccountMenu_CreateAccount(
                    &newAccount,
                    &newData))
            {
                // 创建成功以后，
                // 询问是否进入。

                if (AccountMenu_EnterAccount(
                        &newAccount,
                        &newData,
                        currentAccount,
                        currentData))
                {
                    return true;
                }
            }

            break;

        case 2:

            if (AccountMenu_SelectAccount(
                    currentAccount,
                    currentData))
            {
                // 已经确认进入下一个状态
                return true;
            }

            break;

        case 3:

            running = false;

            break;

        default:

            printf("无效选项！\n");

            break;
        }
    }

    return false;
}
