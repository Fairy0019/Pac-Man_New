#include <stdio.h>
#include "menu.h"

void showMainMenu() {
    printf("==============\n");
    printf("    Pac-Man    \n");
    printf("==============\n");

    printf("1. Start Game/开始游戏\n");
    printf("2. Account Menu/账户菜单\n");
    printf("3. Settings Menu/设置菜单\n");
    printf("4. Exit/退出\n");

    printf("==============\n");
}

MenuChoice getPlayerChoice() {
    int choice;

    printf("Please enter your choice/请选择 (1-4): ");

    scanf("%d", &choice);

    switch (choice) {
        case 1:
            return MENU_START_GAME;
        case 2:
            return MENU_ACCOUNT;
        case 3:
            return MENU_SETTING;
        case 4:
            return MENU_EXIT;
        default:
            printf("Invalid choice. Please try again.\n");
            return getPlayerChoice(); // Recursively ask for input again
    }
}
