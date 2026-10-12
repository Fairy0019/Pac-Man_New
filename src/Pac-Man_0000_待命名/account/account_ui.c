#include "account_ui.h"

#include "raylib.h"

#include <stdbool.h>
#include <string.h>

typedef enum
{
    ACCOUNT_UI_VIEW_HOME = 0,
    ACCOUNT_UI_VIEW_CREATE,
    ACCOUNT_UI_VIEW_SELECT
} AccountUIView;

#define ACCOUNT_UI_HOME_ITEM_COUNT 3
#define ACCOUNT_UI_VISIBLE_ACCOUNTS 8

static AccountUIView currentView = ACCOUNT_UI_VIEW_HOME;
static int selectedItem = 0;
static int selectedAccount = 0;
static int accountScroll = 0;
static PlayerAccount accounts[ACCOUNT_MAX];
static int accountCount = 0;
static char nameInput[ACCOUNT_NAME_MAX + 1] = {0};
static int nameLength = 0;
static char statusMessage[96] = {0};

static const char *const homeLabels[ACCOUNT_UI_HOME_ITEM_COUNT] = {
    "CREATE PROFILE",
    "SELECT PROFILE",
    "QUIT"};

static Rectangle AccountUI_HomeButton(int index)
{
    const float width = 340.0f;
    const float height = 62.0f;
    const float gap = 18.0f;
    const float totalHeight = ACCOUNT_UI_HOME_ITEM_COUNT * height +
                              (ACCOUNT_UI_HOME_ITEM_COUNT - 1) * gap;
    const float firstY = (GetScreenHeight() - totalHeight) / 2.0f + 55.0f;

    return (Rectangle){(GetScreenWidth() - width) / 2.0f,
                       firstY + index * (height + gap), width, height};
}

static Rectangle AccountUI_BackButton(void)
{
    return (Rectangle){(GetScreenWidth() - 220.0f) / 2.0f,
                       GetScreenHeight() - 104.0f, 220.0f, 54.0f};
}

static void AccountUI_OpenSelect(void)
{
    accountCount = Account_List(accounts, ACCOUNT_MAX);
    if (accountCount < 0)
        accountCount = 0;
    if (accountCount > ACCOUNT_MAX)
        accountCount = ACCOUNT_MAX;

    selectedAccount = 0;
    accountScroll = 0;
    statusMessage[0] = '\0';
    currentView = ACCOUNT_UI_VIEW_SELECT;
}

static AccountUIAction AccountUI_EnterSelected(PlayerAccount *account, PlayerData *data)
{
    if (selectedAccount < 0 || selectedAccount >= accountCount)
    {
        strcpy_s(statusMessage, sizeof(statusMessage), "Select a profile first.");
        return ACCOUNT_UI_ACTION_NONE;
    }

    PlayerData loadedData;
    if (!PlayerData_Load(&loadedData, accounts[selectedAccount].uid))
    {
        if (!PlayerData_Create(&loadedData) ||
            !PlayerData_Save(&loadedData, accounts[selectedAccount].uid))
        {
            strcpy_s(statusMessage, sizeof(statusMessage), "Could not load profile data.");
            return ACCOUNT_UI_ACTION_NONE;
        }
    }

    *account = accounts[selectedAccount];
    *data = loadedData;
    return ACCOUNT_UI_ACTION_ENTERED;
}

static AccountUIAction AccountUI_CreateProfile(PlayerAccount *account, PlayerData *data)
{
    if (nameLength == 0)
    {
        strcpy_s(statusMessage, sizeof(statusMessage), "Enter a profile name first.");
        return ACCOUNT_UI_ACTION_NONE;
    }

    PlayerAccount newAccount;
    PlayerData newData;
    if (!Account_Create(&newAccount, nameInput))
    {
        strcpy_s(statusMessage, sizeof(statusMessage), "Could not create profile.");
        return ACCOUNT_UI_ACTION_NONE;
    }

    if (!PlayerData_Create(&newData) || !PlayerData_Save(&newData, newAccount.uid))
    {
        Account_Delete(newAccount.uid);
        PlayerData_Delete(newAccount.uid);
        strcpy_s(statusMessage, sizeof(statusMessage), "Could not save profile data.");
        return ACCOUNT_UI_ACTION_NONE;
    }

    *account = newAccount;
    *data = newData;
    return ACCOUNT_UI_ACTION_ENTERED;
}

void AccountUI_Init(void)
{
    currentView = ACCOUNT_UI_VIEW_HOME;
    selectedItem = 0;
    selectedAccount = 0;
    accountScroll = 0;
    accountCount = 0;
    nameLength = 0;
    nameInput[0] = '\0';
    statusMessage[0] = '\0';
}

AccountUIAction AccountUI_Update(PlayerAccount *account, PlayerData *data)
{
    if (account == NULL || data == NULL)
        return ACCOUNT_UI_ACTION_QUIT;

    Vector2 mouse = GetMousePosition();

    if (currentView == ACCOUNT_UI_VIEW_HOME)
    {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selectedItem = (selectedItem + ACCOUNT_UI_HOME_ITEM_COUNT - 1) % ACCOUNT_UI_HOME_ITEM_COUNT;
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selectedItem = (selectedItem + 1) % ACCOUNT_UI_HOME_ITEM_COUNT;

        for (int i = 0; i < ACCOUNT_UI_HOME_ITEM_COUNT; ++i)
        {
            if (CheckCollisionPointRec(mouse, AccountUI_HomeButton(i)))
            {
                selectedItem = i;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    break;
            }
        }

        bool confirm = IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            for (int i = 0; i < ACCOUNT_UI_HOME_ITEM_COUNT; ++i)
            {
                if (CheckCollisionPointRec(mouse, AccountUI_HomeButton(i)))
                {
                    selectedItem = i;
                    confirm = true;
                    break;
                }
            }
        }

        if (!confirm)
            return ACCOUNT_UI_ACTION_NONE;

        if (selectedItem == 0)
        {
            nameLength = 0;
            nameInput[0] = '\0';
            statusMessage[0] = '\0';
            currentView = ACCOUNT_UI_VIEW_CREATE;
        }
        else if (selectedItem == 1)
            AccountUI_OpenSelect();
        else
            return ACCOUNT_UI_ACTION_QUIT;

        return ACCOUNT_UI_ACTION_NONE;
    }

    if (currentView == ACCOUNT_UI_VIEW_CREATE)
    {
        if (IsKeyPressed(KEY_ESCAPE))
        {
            currentView = ACCOUNT_UI_VIEW_HOME;
            statusMessage[0] = '\0';
            return ACCOUNT_UI_ACTION_NONE;
        }

        if (IsKeyPressed(KEY_BACKSPACE) && nameLength > 0)
        {
            nameInput[--nameLength] = '\0';
        }

        int codepoint = GetCharPressed();
        while (codepoint > 0)
        {
            if (codepoint >= 32 && codepoint <= 126 && nameLength < ACCOUNT_NAME_MAX)
            {
                nameInput[nameLength++] = (char)codepoint;
                nameInput[nameLength] = '\0';
            }
            codepoint = GetCharPressed();
        }

        if (IsKeyPressed(KEY_ENTER))
            return AccountUI_CreateProfile(account, data);

        if (CheckCollisionPointRec(mouse, AccountUI_BackButton()) &&
            IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            currentView = ACCOUNT_UI_VIEW_HOME;
            statusMessage[0] = '\0';
        }
        return ACCOUNT_UI_ACTION_NONE;
    }

    if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE))
    {
        currentView = ACCOUNT_UI_VIEW_HOME;
        statusMessage[0] = '\0';
        return ACCOUNT_UI_ACTION_NONE;
    }

    if (accountCount > 0)
    {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
            selectedAccount = (selectedAccount + accountCount - 1) % accountCount;
        else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
            selectedAccount = (selectedAccount + 1) % accountCount;

        if (selectedAccount < accountScroll)
            accountScroll = selectedAccount;
        else if (selectedAccount >= accountScroll + ACCOUNT_UI_VISIBLE_ACCOUNTS)
            accountScroll = selectedAccount - ACCOUNT_UI_VISIBLE_ACCOUNTS + 1;

        int visibleCount = accountCount - accountScroll;
        if (visibleCount > ACCOUNT_UI_VISIBLE_ACCOUNTS)
            visibleCount = ACCOUNT_UI_VISIBLE_ACCOUNTS;
        float rowHeight = 54.0f;
        float firstY = 205.0f;
        for (int i = 0; i < visibleCount; ++i)
        {
            Rectangle row = {(GetScreenWidth() - 640.0f) / 2.0f,
                             firstY + i * (rowHeight + 8.0f), 640.0f, rowHeight};
            if (CheckCollisionPointRec(mouse, row))
            {
                selectedAccount = accountScroll + i;
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    return AccountUI_EnterSelected(account, data);
            }
        }

        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
            return AccountUI_EnterSelected(account, data);
    }

    if (CheckCollisionPointRec(mouse, AccountUI_BackButton()) &&
        IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        currentView = ACCOUNT_UI_VIEW_HOME;
        statusMessage[0] = '\0';
    }

    return ACCOUNT_UI_ACTION_NONE;
}

void AccountUI_Draw(void)
{
    ClearBackground((Color){12, 18, 38, 255});
    const char *title = "PLAYER PROFILE";
    DrawText(title, (GetScreenWidth() - MeasureText(title, 48)) / 2, 76, 48, GOLD);

    if (currentView == ACCOUNT_UI_VIEW_HOME)
    {
        for (int i = 0; i < ACCOUNT_UI_HOME_ITEM_COUNT; ++i)
        {
            Rectangle bounds = AccountUI_HomeButton(i);
            bool selected = i == selectedItem;
            Color fill = selected ? (Color){255, 204, 0, 255} : (Color){35, 49, 79, 255};
            Color text = selected ? (Color){20, 26, 42, 255} : RAYWHITE;
            DrawRectangleRounded(bounds, 0.22f, 8, fill);
            DrawText(homeLabels[i], (int)(bounds.x + (bounds.width - MeasureText(homeLabels[i], 23)) / 2),
                     (int)(bounds.y + 19), 23, text);
        }
    }
    else if (currentView == ACCOUNT_UI_VIEW_CREATE)
    {
        const char *prompt = "ENTER PROFILE NAME";
        DrawText(prompt, (GetScreenWidth() - MeasureText(prompt, 24)) / 2, 255, 24, RAYWHITE);
        Rectangle input = {(GetScreenWidth() - 520.0f) / 2.0f, 305.0f, 520.0f, 64.0f};
        DrawRectangleRounded(input, 0.15f, 6, (Color){35, 49, 79, 255});
        DrawText(nameInput[0] ? nameInput : "Type a name...",
                 (int)input.x + 20, (int)input.y + 19, 24,
                 nameInput[0] ? RAYWHITE : (Color){160, 170, 190, 255});
        DrawText("ENTER: CREATE AND CONTINUE     ESC: BACK",
                 (GetScreenWidth() - MeasureText("ENTER: CREATE AND CONTINUE     ESC: BACK", 18)) / 2,
                 410, 18, (Color){170, 184, 211, 255});
    }
    else
    {
        if (accountCount == 0)
        {
            const char *empty = "NO PROFILES YET";
            DrawText(empty, (GetScreenWidth() - MeasureText(empty, 24)) / 2,
                     GetScreenHeight() / 2 - 20, 24, RAYWHITE);
        }

        int visibleCount = accountCount - accountScroll;
        if (visibleCount > ACCOUNT_UI_VISIBLE_ACCOUNTS)
            visibleCount = ACCOUNT_UI_VISIBLE_ACCOUNTS;
        for (int i = 0; i < visibleCount; ++i)
        {
            Rectangle row = {(GetScreenWidth() - 640.0f) / 2.0f,
                             205.0f + i * 62.0f, 640.0f, 54.0f};
            int accountIndex = accountScroll + i;
            bool selected = accountIndex == selectedAccount;
            DrawRectangleRounded(row, 0.15f, 6,
                                 selected ? (Color){255, 204, 0, 255} : (Color){35, 49, 79, 255});
            DrawText(accounts[accountIndex].name, (int)row.x + 18, (int)row.y + 17, 20,
                     selected ? (Color){20, 26, 42, 255} : RAYWHITE);
            DrawText(accounts[accountIndex].uid, (int)(row.x + row.width - MeasureText(accounts[accountIndex].uid, 16) - 18),
                     (int)row.y + 19, 16,
                     selected ? (Color){20, 26, 42, 255} : (Color){170, 184, 211, 255});
        }
        DrawText("ENTER: SELECT PROFILE", (GetScreenWidth() - MeasureText("ENTER: SELECT PROFILE", 18)) / 2,
                 GetScreenHeight() - 145, 18, (Color){170, 184, 211, 255});
    }

    if (statusMessage[0] != '\0')
        DrawText(statusMessage, (GetScreenWidth() - MeasureText(statusMessage, 18)) / 2,
                 GetScreenHeight() - 150, 18, RED);

    if (currentView != ACCOUNT_UI_VIEW_HOME)
    {
        Rectangle back = AccountUI_BackButton();
        DrawRectangleRounded(back, 0.2f, 6, (Color){35, 49, 79, 255});
        DrawText("BACK", (int)(back.x + (back.width - MeasureText("BACK", 20)) / 2),
                 (int)back.y + 16, 20, RAYWHITE);
    }
}
