#pragma once

#ifndef SETTINGS_H
#define SETTINGS_H

typedef enum
{
    SETTINGS_ACTION_NONE = 0,
    SETTINGS_ACTION_BACK
} SettingsAction;

// 在 raylib 窗口创建后调用，重置设置页的选择位置。
// 音量默认值在本次程序运行期间保留；目前尚未写入配置文件。
void Settings_Init(void);

// 更新键盘/鼠标输入；选择“返回”时返回 SETTINGS_ACTION_BACK。
SettingsAction Settings_Update(void);

// 在已打开的 raylib 窗口中绘制设置页面。
void Settings_Draw(void);

float Settings_GetMusicVolume(void);
float Settings_GetSfxVolume(void);

#endif
