#include "raylib.h"

#include "Game_State/Game_State.h"
#include "account/account.h"
#include "account/account_ui.h"
#include "account/history.h"
#include "account/player_data.h"
#include "audio/audio_manager.h"
#include "game/game.h"
#include "main_menu/main_menu.h"
#include "settings/settings.h"
#include "texture/texture.h"
#include "collision/draw.h"

#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

static PlayerAccount currentAccount = {0};
static PlayerData currentData = {0};
static GameHistory *historyRecords = NULL;
static int historyRecordCount = 0;
static int historyScroll = 0;
static time_t gameStartTime = 0;
static bool gameHistorySaved = false;

static Rectangle Main_HistoryBackButton(void)
{
    return (Rectangle){(GetScreenWidth() - 220.0f) / 2.0f,
                       GetScreenHeight() - 104.0f, 220.0f, 54.0f};
}

static void Main_ReturnFromHistory(void)
{
    free(historyRecords);
    historyRecords = NULL;
    historyRecordCount = 0;
    setGameState(STATE_MAIN_MENU);
}

static void Main_LoadHistory(void)
{
    free(historyRecords);
    historyRecords = NULL;
    historyRecordCount = History_LoadAll(currentAccount.uid, &historyRecords);
    if (historyRecordCount < 0)
        historyRecordCount = 0;
    historyScroll = 0;
}

static void Main_SaveFinishedGame(void)
{
    if (gameHistorySaved || (!gameOver && !win))
        return;

    GameHistory record = {0};
    record.mode = 1;
    record.score = score;
    record.startTime = gameStartTime;
    record.durationSeconds = (int)difftime(time(NULL), gameStartTime);
    record.result = win ? 1 : 0;

    if (History_Add(currentAccount.uid, &record))
        gameHistorySaved = true;
}

static void MainMenu_HandleAction(MainMenuAction action)
{
    switch (action)
    {
    case MAIN_MENU_ACTION_START_GAME:
        setGameState(STATE_MODE_SELECT);
        break;

    case MAIN_MENU_ACTION_HISTORY:
        Main_LoadHistory();
        setGameState(STATE_HISTORY);
        break;

    case MAIN_MENU_ACTION_SETTINGS:
        setGameState(STATE_SETTING);
        break;

    case MAIN_MENU_ACTION_EXIT:
        setGameState(STATE_EXIT);
        break;

    case MAIN_MENU_ACTION_NONE:
    default:
        break;
    }
}

static void Main_Update(float deltaTime)
{
    switch (getGameState())
    {
    case STATE_MAIN_MENU:
        MainMenu_HandleAction(MainMenu_Update());
        break;

    case STATE_PROFILE:
    {
        AccountUIAction action = AccountUI_Update(&currentAccount, &currentData);
        if (action == ACCOUNT_UI_ACTION_ENTERED)
            setGameState(STATE_MAIN_MENU);
        else if (action == ACCOUNT_UI_ACTION_QUIT)
            setGameState(STATE_EXIT);
        break;
    }

    case STATE_SETTING:
        if (Settings_Update() == SETTINGS_ACTION_BACK)
            setGameState(STATE_MAIN_MENU);
        break;

    case STATE_GAME:
        Main_SaveFinishedGame();
        if (IsKeyPressed(KEY_Q))
            setGameState(STATE_EXIT);
        else if (IsKeyPressed(KEY_ESCAPE))
            setGameState(STATE_PAUSE);
        else if (IsKeyPressed(KEY_R))
        {
            initGame();
            gameStartTime = time(NULL);
            gameHistorySaved = false;
        }
        else
        {
            updateGame(deltaTime);
            Main_SaveFinishedGame();
        }
        break;

    case STATE_PAUSE:
        if (IsKeyPressed(KEY_Q))
            setGameState(STATE_EXIT);
        else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE))
            setGameState(STATE_GAME);
        else if (IsKeyPressed(KEY_M))
            setGameState(STATE_MAIN_MENU);
        break;

    case STATE_MODE_SELECT:
        if (IsKeyPressed(KEY_ENTER))
        {
            initGame();
            gameStartTime = time(NULL);
            gameHistorySaved = false;
            setGameState(STATE_GAME);
        }
        else if (IsKeyPressed(KEY_ESCAPE))
            setGameState(STATE_MAIN_MENU);
        break;

    case STATE_HISTORY:
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE) ||
            (CheckCollisionPointRec(GetMousePosition(), Main_HistoryBackButton()) &&
             IsMouseButtonPressed(MOUSE_BUTTON_LEFT)))
            Main_ReturnFromHistory();
        else if (IsKeyPressed(KEY_UP) && historyScroll > 0)
            --historyScroll;
        else if (IsKeyPressed(KEY_DOWN) && historyScroll + 6 < historyRecordCount)
            ++historyScroll;
        break;

    case STATE_EXIT:
    case MAX_STATES:
    default:
        break;
    }
}

static void Main_Draw(void)
{
    BeginDrawing();

    switch (getGameState())
    {
    case STATE_MAIN_MENU:
        MainMenu_Draw();
        break;

    case STATE_SETTING:
        Settings_Draw();
        break;

    case STATE_HISTORY:
    {
        ClearBackground((Color){12, 18, 38, 255});
        const char *title = "GAME HISTORY";
        DrawText(title, (GetScreenWidth() - MeasureText(title, 48)) / 2, 74, 48, GOLD);

        if (historyRecordCount == 0)
        {
            const char *emptyMessage = "NO GAMES RECORDED";
            DrawText(emptyMessage,
                     (GetScreenWidth() - MeasureText(emptyMessage, 24)) / 2,
                     GetScreenHeight() / 2 - 12, 24, RAYWHITE);
        }
        else
        {
            const int visibleRows = 6;
            int rows = historyRecordCount - historyScroll;
            if (rows > visibleRows)
                rows = visibleRows;

            for (int i = 0; i < rows; ++i)
            {
                int recordIndex = historyRecordCount - 1 - (historyScroll + i);
                GameHistory record = historyRecords[recordIndex];
                struct tm localTime;
                char timeText[32] = "Unknown time";
                if (localtime_s(&localTime, &record.startTime) == 0)
                    strftime(timeText, sizeof(timeText), "%Y-%m-%d %H:%M", &localTime);

                DrawText(TextFormat("%s    %s    SCORE %d    %d SEC",
                                    timeText,
                                    record.result ? "WIN" : "GAME OVER",
                                    record.score,
                                    record.durationSeconds),
                         120, 190 + i * 76, 22, RAYWHITE);
            }
        }

        const char *hint = "UP / DOWN to browse    ESC / BACKSPACE to return";
        DrawText(hint, (GetScreenWidth() - MeasureText(hint, 18)) / 2,
                 GetScreenHeight() - 42, 18, (Color){170, 184, 211, 255});
        Rectangle back = Main_HistoryBackButton();
        DrawRectangleRounded(back, 0.2f, 6, (Color){35, 49, 79, 255});
        DrawText("BACK TO MENU",
                 (int)(back.x + (back.width - MeasureText("BACK TO MENU", 18)) / 2),
                 (int)back.y + 18, 18, RAYWHITE);
        break;
    }

    case STATE_GAME:
        drawGame();
        break;

    case STATE_PAUSE:
        drawGame();
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), (Color){0, 0, 0, 170});
        DrawText("PAUSED", GetScreenWidth() / 2 - MeasureText("PAUSED", 48) / 2,
                 GetScreenHeight() / 2 - 70, 48, GOLD);
        DrawText("ESC / ENTER: RESUME     M: MAIN MENU     Q: QUIT",
                 GetScreenWidth() / 2 - MeasureText("ESC / ENTER: RESUME     M: MAIN MENU     Q: QUIT", 20) / 2,
                 GetScreenHeight() / 2 + 8, 20, RAYWHITE);
        break;

    case STATE_MODE_SELECT:
        ClearBackground((Color){12, 18, 38, 255});
        DrawText("ONE GAME MODE AVAILABLE", GetScreenWidth() / 2 - 240,
                 GetScreenHeight() / 2 - 30, 32, GOLD);
        DrawText("ENTER: START     ESC: BACK", GetScreenWidth() / 2 - 170,
                 GetScreenHeight() / 2 + 30, 20, RAYWHITE);
        break;

    case STATE_PROFILE:
        AccountUI_Draw();
        break;

    case STATE_EXIT:
    case MAX_STATES:
    default:
        ClearBackground(BLACK);
        break;
    }

    EndDrawing();
}

int main(void)
{
    srand((unsigned)time(NULL));

    if (!Account_Init())
        return 1;

    if (!PlayerData_Init())
    {
        Account_Shutdown();
        return 1;
    }

    if (!History_Init())
    {
        PlayerData_Shutdown();
        Account_Shutdown();
        return 1;
    }

    InitWindow(WIDTH * TILE_SIZE, HEIGHT * TILE_SIZE + UI_HEIGHT, "Pac-Man");
    // raylib 默认用 Esc 关闭窗口；Esc 应由当前 GameState 处理暂停/返回。
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    if (!AudioManager_Init("Music/HOYO-MiX - 太空漫步 Space Walk.mp3"))
        TraceLog(LOG_WARNING, "Background music could not be loaded; continuing without music.");

    loadTextures();
    initGame();
    MainMenu_Init();
    Settings_Init();
    AccountUI_Init();
    AudioManager_SetMusicVolume(Settings_GetMusicVolume());

    while (!WindowShouldClose() && getGameState() != STATE_EXIT)
    {
        Main_Update(GetFrameTime());
        AudioManager_Update();
        AudioManager_SetMusicVolume(Settings_GetMusicVolume());
        Main_Draw();
    }

    AudioManager_Shutdown();
    unloadTextures();
    CloseWindow();
    History_Shutdown();
    free(historyRecords);
    PlayerData_Shutdown();
    Account_Shutdown();

    return 0;
}
