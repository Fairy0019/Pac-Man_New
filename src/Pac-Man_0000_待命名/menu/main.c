#include <stdio.h>

#include "menu.h"

int main()
{
    while(1)
    {
        showMainMenu();

        MenuChoice choice = getPlayerChoice();


        switch(choice)
        {
            case MENU_START_GAME:
                printf("Starting game...\n");
                // Add game starting logic here
                break;

            case MENU_ACCOUNT:
                printf("Opening account menu...\n");
                // Add account menu logic here
                break;
            
            case MENU_SETTING:
                printf("Opening settings menu...\n");
                // Add settings menu logic here
                break;
                
            case MENU_EXIT:
                printf("Exiting the game. Goodbye!\n");
                return 0; // Exit the program

        }
        
    }

}