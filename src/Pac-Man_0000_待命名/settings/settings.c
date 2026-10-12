#include "settings.h"

#include "raylib.h"

#define SETTINGS_ITEM_COUNT 3
#define SETTINGS_SLIDER_WIDTH 220.0f
#define SETTINGS_SLIDER_HEIGHT 12.0f

typedef enum
{
    SETTINGS_ITEM_MUSIC_VOLUME = 0,
    SETTINGS_ITEM_SFX_VOLUME,
    SETTINGS_ITEM_BACK
} SettingsItem;

static int selectedItem = SETTINGS_ITEM_MUSIC_VOLUME;
static float musicVolume = 0.70f;
static float sfxVolume = 0.80f;

static float Settings_ClampVolume(float value)
{
    if (value < 0.0f)
        return 0.0f;
    if (value > 1.0f)
        return 1.0f;
    return value;
}

static Rectangle Settings_GetRowBounds(int index)
{
    const float rowWidth = 560.0f;
    const float rowHeight = 64.0f;
    const float rowGap = 16.0f;
    const float totalHeight = SETTINGS_ITEM_COUNT * rowHeight +
                              (SETTINGS_ITEM_COUNT - 1) * rowGap;
    const float firstY = (GetScreenHeight() - totalHeight) / 2.0f + 60.0f;

    return (Rectangle){
        (GetScreenWidth() - rowWidth) / 2.0f,
        firstY + index * (rowHeight + rowGap),
        rowWidth,
        rowHeight};
}

static void Settings_AdjustSelectedVolume(float amount)
{
    if (selectedItem == SETTINGS_ITEM_MUSIC_VOLUME)
    {
        musicVolume = Settings_ClampVolume(musicVolume + amount);
    }
    else if (selectedItem == SETTINGS_ITEM_SFX_VOLUME)
    {
        sfxVolume = Settings_ClampVolume(sfxVolume + amount);
    }
}

static void Settings_SetVolumeFromMouse(int item, float mouseX)
{
    Rectangle row = Settings_GetRowBounds(item);
    float sliderX = row.x + row.width - SETTINGS_SLIDER_WIDTH - 24.0f;
    float ratio = (mouseX - sliderX) / SETTINGS_SLIDER_WIDTH;
    float value = Settings_ClampVolume(ratio);

    if (item == SETTINGS_ITEM_MUSIC_VOLUME)
        musicVolume = value;
    else
        sfxVolume = value;
}

void Settings_Init(void)
{
    selectedItem = SETTINGS_ITEM_MUSIC_VOLUME;
}

SettingsAction Settings_Update(void)
{
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        selectedItem = (selectedItem + SETTINGS_ITEM_COUNT - 1) % SETTINGS_ITEM_COUNT;
    else if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        selectedItem = (selectedItem + 1) % SETTINGS_ITEM_COUNT;

    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        Settings_AdjustSelectedVolume(-0.05f);
    else if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        Settings_AdjustSelectedVolume(0.05f);

    Vector2 mouse = GetMousePosition();
    for (int i = 0; i < SETTINGS_ITEM_COUNT; ++i)
    {
        Rectangle row = Settings_GetRowBounds(i);
        if (!CheckCollisionPointRec(mouse, row))
            continue;

        selectedItem = i;
        if (!IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            continue;

        if (i == SETTINGS_ITEM_BACK)
            return SETTINGS_ACTION_BACK;

        Settings_SetVolumeFromMouse(i, mouse.x);
    }

    if (selectedItem == SETTINGS_ITEM_BACK &&
        (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ESCAPE)))
        return SETTINGS_ACTION_BACK;

    if (IsKeyPressed(KEY_ESCAPE))
        return SETTINGS_ACTION_BACK;

    return SETTINGS_ACTION_NONE;
}

void Settings_Draw(void)
{
    const int screenWidth = GetScreenWidth();
    const char *title = "SETTINGS";
    const int titleSize = 48;

    ClearBackground((Color){12, 18, 38, 255});
    DrawText(title,
             (screenWidth - MeasureText(title, titleSize)) / 2,
             GetScreenHeight() / 2 - 210,
             titleSize,
             GOLD);

    const char *labels[SETTINGS_ITEM_COUNT] = {
        "MUSIC VOLUME",
        "SOUND EFFECTS",
        "BACK"};
    const float values[2] = {musicVolume, sfxVolume};

    for (int i = 0; i < SETTINGS_ITEM_COUNT; ++i)
    {
        Rectangle row = Settings_GetRowBounds(i);
        bool selected = (i == selectedItem);
        Color fill = selected ? (Color){255, 204, 0, 255} : (Color){35, 49, 79, 255};
        Color foreground = selected ? (Color){20, 26, 42, 255} : RAYWHITE;

        DrawRectangleRounded(row, 0.18f, 8, fill);
        DrawText(labels[i], (int)row.x + 24, (int)row.y + 21, 21, foreground);

        if (i < 2)
        {
            float sliderX = row.x + row.width - SETTINGS_SLIDER_WIDTH - 24.0f;
            float sliderY = row.y + row.height / 2.0f;
            Rectangle track = {sliderX, sliderY - SETTINGS_SLIDER_HEIGHT / 2.0f,
                               SETTINGS_SLIDER_WIDTH, SETTINGS_SLIDER_HEIGHT};
            Rectangle fillBar = track;
            fillBar.width *= values[i];

            DrawRectangleRounded(track, 0.5f, 6, (Color){95, 108, 132, 255});
            if (fillBar.width > 0.0f)
                DrawRectangleRounded(fillBar, 0.5f, 6, selected ? (Color){35, 49, 79, 255} : GOLD);
            DrawCircle((int)(sliderX + SETTINGS_SLIDER_WIDTH * values[i]),
                       (int)sliderY, 9.0f, foreground);
            DrawText(TextFormat("%d%%", (int)(values[i] * 100.0f + 0.5f)),
                     (int)(sliderX + SETTINGS_SLIDER_WIDTH + 12.0f),
                     (int)row.y + 22,
                     18,
                     foreground);
        }
    }

    const char *hint = "UP / DOWN to select    LEFT / RIGHT to adjust    ESC to return";
    DrawText(hint,
             (screenWidth - MeasureText(hint, 18)) / 2,
             GetScreenHeight() - 42,
             18,
             (Color){170, 184, 211, 255});
}

float Settings_GetMusicVolume(void)
{
    return musicVolume;
}

float Settings_GetSfxVolume(void)
{
    return sfxVolume;
}
