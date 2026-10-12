#include "main_menu.h"

#include "raylib.h"

#define MAIN_MENU_ITEM_COUNT 4
#define MAIN_MENU_BUTTON_WIDTH 280
#define MAIN_MENU_BUTTON_HEIGHT 54
#define MAIN_MENU_BUTTON_GAP 14

static const char *const menuLabels[MAIN_MENU_ITEM_COUNT] = {
    "START GAME",
    "SETTINGS",
    "ACCOUNT",
    "EXIT GAME"};

static const MainMenuAction menuActions[MAIN_MENU_ITEM_COUNT] = {
    MAIN_MENU_ACTION_START_GAME,
    MAIN_MENU_ACTION_SETTINGS,
    MAIN_MENU_ACTION_ACCOUNT,
    MAIN_MENU_ACTION_EXIT};

static int selectedItem = 0;

static Rectangle MainMenu_GetButtonBounds(int index)
{
    const int totalHeight = MAIN_MENU_ITEM_COUNT * MAIN_MENU_BUTTON_HEIGHT +
                            (MAIN_MENU_ITEM_COUNT - 1) * MAIN_MENU_BUTTON_GAP;
    const int firstY = (GetScreenHeight() - totalHeight) / 2 + 45;

    return (Rectangle){
        (float)(GetScreenWidth() - MAIN_MENU_BUTTON_WIDTH) / 2.0f,
        (float)(firstY + index * (MAIN_MENU_BUTTON_HEIGHT + MAIN_MENU_BUTTON_GAP)),
        (float)MAIN_MENU_BUTTON_WIDTH,
        (float)MAIN_MENU_BUTTON_HEIGHT};
}

void MainMenu_Init(void)
{
    selectedItem = 0;
}

MainMenuAction MainMenu_Update(void)
{
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
    {
        selectedItem = (selectedItem + MAIN_MENU_ITEM_COUNT - 1) % MAIN_MENU_ITEM_COUNT;
    }
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
    {
        selectedItem = (selectedItem + 1) % MAIN_MENU_ITEM_COUNT;
    }

    Vector2 mousePosition = GetMousePosition();
    for (int i = 0; i < MAIN_MENU_ITEM_COUNT; ++i)
    {
        if (CheckCollisionPointRec(mousePosition, MainMenu_GetButtonBounds(i)))
        {
            selectedItem = i;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                return menuActions[i];
        }
    }

    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE))
        return menuActions[selectedItem];

    if (IsKeyPressed(KEY_ESCAPE))
        return MAIN_MENU_ACTION_EXIT;

    return MAIN_MENU_ACTION_NONE;
}

void MainMenu_Draw(void)
{
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const char *title = "PAC-MAN";
    const int titleFontSize = 64;
    const int titleWidth = MeasureText(title, titleFontSize);
    const int titleY = screenHeight / 2 - 220;

    ClearBackground((Color){12, 18, 38, 255});
    DrawCircle(screenWidth / 2, titleY + 25, 115.0f, (Color){22, 36, 69, 255});
    DrawText(title, (screenWidth - titleWidth) / 2 + 3, titleY + 3,
             titleFontSize, (Color){10, 10, 18, 180});
    DrawText(title, (screenWidth - titleWidth) / 2, titleY,
             titleFontSize, GOLD);

    const char *subtitle = "READY PLAYER ONE";
    const int subtitleFontSize = 24;
    DrawText(subtitle,
             (screenWidth - MeasureText(subtitle, subtitleFontSize)) / 2,
             titleY + 72,
             subtitleFontSize,
             RAYWHITE);

    for (int i = 0; i < MAIN_MENU_ITEM_COUNT; ++i)
    {
        Rectangle bounds = MainMenu_GetButtonBounds(i);
        bool selected = (i == selectedItem);
        Color fill = selected ? (Color){255, 204, 0, 255} : (Color){35, 49, 79, 255};
        Color textColor = selected ? (Color){20, 26, 42, 255} : RAYWHITE;
        int fontSize = 24;
        int textWidth = MeasureText(menuLabels[i], fontSize);

        DrawRectangleRounded(bounds, 0.25f, 8, fill);
        if (selected)
            DrawRectangleRoundedLinesEx(bounds, 0.25f, 8, 2.0f, (Color){255, 236, 130, 255});
        DrawText(menuLabels[i],
                 (int)(bounds.x + (bounds.width - textWidth) / 2),
                 (int)(bounds.y + (bounds.height - fontSize) / 2),
                 fontSize,
                 textColor);
    }

    const char *hint = "UP / DOWN or W / S to select    ENTER to confirm";
    DrawText(hint,
             (screenWidth - MeasureText(hint, 18)) / 2,
             screenHeight - 42,
             18,
             (Color){170, 184, 211, 255});
}
