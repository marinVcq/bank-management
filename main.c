#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "customer.h"
#include "interface.h"

int exit_status = 0;
void handle_selection(int selector);

int main()
{
    char main_option_arr[3][50] = {"CREER UN COMPTE", "ACCEDER A MON ESPACE", "QUITTER"};
    int selected_option = 1;

    /* Setup the screen buffer */
    setup_console();

    while (exit_status == 0)
    {
        /* Event listener */
        ReadConsoleInput(hStdin, &rc, 1, &dwRead);

        if (rc.Event.KeyEvent.bKeyDown == FALSE)
        {
            switch (rc.Event.KeyEvent.wVirtualKeyCode)
            {
            case 0x51:
                exit_status = 1;
                break;
            case VK_UP:
                if (selected_option - 1 >= 1)
                {
                    selected_option -= 1;
                }
                else
                {
                    selected_option = 3;
                }
                break;
            case VK_DOWN:
                if (selected_option + 1 <= 3)
                {
                    selected_option += 1;
                }
                else
                {
                    selected_option = 1;
                }
                break;
            case VK_RETURN:
                handle_selection(selected_option); // Thread
                break;
            }
        }

        /* Display */
        system("cls");
        print_image(40 - 24, "title-image.txt");
        line_break(4);
        print_color("\xB2\xB2\xB2\xB2 EFFECTUER UNE OPERATION \xB2\xB2\xB2\xB2\n\n", "YELLOW", MIDDLE_WIDTH - 17);
        line_break(3);
        list_option(main_option_arr, selected_option, 3);
        line_break(2);
        print_color("\xB2\xB2\xB2\xB2 PRESS ENTER TO SELECT \xB2\xB2\xB2\xB2\n\n", "YELLOW", MIDDLE_WIDTH - 16);
    }

    /* Restore user's console parameters */
    restore_console();

    /* Clear console */
    // system("cls");
}

void handle_selection(int selector)
{
    switch (selector)
    {
    case 1:
        create_account();
        break;
    case 2:
        login_account();
        break;
    case 3:
        exit_status = 1;
        break;
    }
}
