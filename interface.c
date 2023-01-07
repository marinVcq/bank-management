#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <process.h>
#include "interface.h"
#include "customer.h"

/* Define GLOBAL */
CONSOLE_SCREEN_BUFFER_INFO default_console_info = {0};
WORD default_text_attributes = 0;
DWORD dwRead = 0;
HANDLE hStdout = NULL;
HANDLE hStdin = NULL;
INPUT_RECORD rc = {0};
CONSOLE_CURSOR_INFO cursor_info = {0};

void setTimeout(int delay_msec)
{
    int msec = 0;
    clock_t before = clock();

    while (msec < delay_msec)
    {
        clock_t diff = clock() - before;
        msec = diff * 1000 / CLOCKS_PER_SEC;
    }
}

void print_color(char *string, char *color, int margin)
{
    CONSOLE_SCREEN_BUFFER_INFO temp;
    /* Store current wAtributes */
    if (GetConsoleScreenBufferInfo(hStdout, &temp) != 0)
    {
        if (strcmp(color, "GREEN") == 0)
        {
            print_spaces(margin);
            SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY | FOREGROUND_GREEN);
            printf("%s", string);
        }
        else if (strcmp(color, "RED") == 0)
        {
            print_spaces(margin);
            SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY | FOREGROUND_RED);
            printf("%s", string);
        }
        else if (strcmp(color, "YELLOW") == 0)
        {
            print_spaces(margin);
            SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            printf("%s", string);
        }
        else if (strcmp(color, "WHITE") == 0)
        {
            print_spaces(margin);
            SetConsoleTextAttribute(hStdout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
            printf("%s", string);
        }
    }
    /* Reset color */
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), temp.wAttributes);
}

void line_break(int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("\n");
    }
}

void setup_console(void)
{
    /* Define HANDLE */
    if ((hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
    {
        DWORD lasterror = GetLastError();
        fprintf(stderr, "Error in GetStdHandle(STD_OUTPUT_HANDLE): %s\n", lasterror);
    }
    else
    {
        print_color("Output handle sucessfully setup!\n", "GREEN", 20);
    }

    if ((hStdin = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
    {
        DWORD lasterror = GetLastError();
        fprintf(stderr, "Error in GetStdHandle(STD_INPUT_HANDLE): %s\n", lasterror);
    }
    else
    {
        print_color("Input handle sucessfully setup!\n", "GREEN", 20);
    }

    /* Save default parameters  */
    if (GetConsoleScreenBufferInfo(hStdout, &default_console_info) == 0)
    {
        DWORD lasterror = GetLastError();
        fprintf(stderr, "Error in GetConsoleScreenBufferInfo: %s\n", lasterror);
    }
    else
    {
        print_color("Get console default parameters sucessfully!\n", "GREEN", 20);
    }

    default_text_attributes = default_console_info.wAttributes;

    /* Set console program title */
    SetConsoleTitle("C-ATM Management by Marinos");

    /* Set window and buffer size */
    COORD coord;
    coord.X = CONSOLE_WIDTH;
    coord.Y = CONSOLE_HEIGHT;

    SMALL_RECT rect;
    rect.Top = 0;
    rect.Left = 0;
    rect.Bottom = CONSOLE_HEIGHT - 1;
    rect.Right = CONSOLE_WIDTH - 1;

    if (SetConsoleWindowInfo(hStdout, TRUE, &rect) == 0)
    {
        DWORD lasterror = GetLastError();
        fprintf(stderr, "Error in SetConsoleWindowInfo: %s\n", lasterror);
    }
    else
    {
        print_color("Setup console window size successfully!\n", "GREEN", 20);
    }
    if (SetConsoleScreenBufferSize(hStdout, coord) == 0)
    {
        DWORD lasterror = GetLastError();
        fprintf(stderr, "Error in SetConsoleScreenBufferSize: %s\n", lasterror);
    }
    else
    {
        print_color("Setup console screen buffer size successfully!\n", "GREEN", 20);
    }

    /* Set console font */
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 16;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = 400; // FW_NORMAL
    wcscpy(cfi.FaceName, L"Consolas");
    if (SetCurrentConsoleFontEx(hStdout, FALSE, &cfi) == 0)
    {
        DWORD lasterror = GetLastError();
        fprintf(stderr, "Error in SetCurrentConsoleFontEx: %s\n", lasterror);
    }
    else
    {
        print_color("Setup console font successfully!\n", "GREEN", 20);
    }

    /* Parametrage des modes d'utilisation de la mémoire tampon */
    DWORD dwOutMode = 0;
    DWORD dwInMode = 0;

    GetConsoleMode(hStdout, &dwOutMode);
    GetConsoleMode(hStdin, &dwInMode);

    /**
     *  Add mods:
     *  - dwOutMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
     *  - dwInMode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
     */

    SetConsoleMode(hStdout, dwOutMode);
    SetConsoleMode(hStdin, dwInMode);

    /* Disable cursor  */
    if (GetConsoleCursorInfo(hStdout, &cursor_info) == 0)
    {
        DWORD lasterror = GetLastError();
        fprintf(stderr, "Error in SetConsoleCursorInfo: %s\n", lasterror);
    }
    else
    {
        print_color("Get cursor information successfully!\n", "GREEN", 20);
    }
    cursor_info.bVisible = FALSE;
    if (SetConsoleCursorInfo(hStdout, &cursor_info) == 0)
    {
        DWORD lasterror = GetLastError();
        fprintf(stderr, "Error in SetConsoleCursorInfo: %s\n", lasterror);
    }
    else
    {
        print_color("Setup console cursor successfully!\n", "GREEN", 20);
    }
}

void print_spaces(int n)
{
    for (int i = 0; i < n; i++)
        printf(" ");
}

void print_date(void)
{
    time_t now = time(NULL);
    struct tm tm = *localtime(&now);
    printf("\nLe %02d-%02d-%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void fordelay(int j)
{
    int i, k;
    for (i = 0; i < j; i++)
        k = i;
}

void print_image(int x_pos, char *fn)
{

    FILE *file;
    char *filename = fn;

    if ((file = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "impossible d'ouvrir le fichier : %s\n", filename);
    }
    else
    {
        char read_string[256];
        while (fgets(read_string, sizeof(read_string), file) != NULL)
        {
            print_spaces(x_pos);
            printf("%s", read_string);
        }
    }

    fclose(file);
}

void ResetConsoleColour(WORD Attributes)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

void loading_screen()
{

    int columns_by_2 = MIDDLE_WIDTH;
    system("cls");
    print_spaces(MIDDLE_WIDTH - 18);
    print_image(MIDDLE_WIDTH - 18, "title-image.txt");
    printf("\n\n\n\n\n\n\n\n");
    print_spaces(columns_by_2 - 3);
    printf("LOADING");
    for (int i = 0; i <= 6; i++)
    {
        fordelay(200000000);
        printf(".");
    }
    system("cls");
}

void list_option(char opt_arr[][50], int selector, int size)
{

    for (int i = 0; i < size; i++)
    {
        if ((i + 1) == selector)
        {
            print_spaces(MIDDLE_WIDTH - 12);
            SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
            printf("> %d. %s\n\n", (i + 1), opt_arr[i]);
            ResetConsoleColour(default_text_attributes);
        }
        else
        {
            print_spaces(MIDDLE_WIDTH - 12);
            printf("%d. %s\n\n", (i + 1), opt_arr[i]);
        }
    }
}
int menu_option = 1;

void menu(void *name)
{
    loading_screen();

    char menu_option_arr[6][50] = {"EFFECTUER UN RETRAIT", "CONSULTER MON SOLDE", "EFFECTUER UN VIREMENT", "AFFICHER MES INFORMATIONS", "MODIFIER MON MOT DE PASSE", "TERMINER L'OPERATION EN COURS"};
    int menu_is_running = 1;

    while (menu_is_running)
    {

        // Wait for display to be available, then lock it.
        WaitForSingleObject(hScreenMutex, INFINITE);

        system("cls");
        print_image(MIDDLE_WIDTH - 24, "title-image.txt");
        line_break(2);
        print_color("\xB2\xB2\xB2\xB2 BIENVENUE DANS VOTRE ESPACE \xB2\xB2\xB2\xB2", "YELLOW", MIDDLE_WIDTH - 17);
        line_break(3);
        print_color("\xB2\xB2\xB2\xB2 EFFECTUER UNE OPERATION \xB2\xB2\xB2\xB2", "YELLOW", MIDDLE_WIDTH - 17);
        line_break(2);
        list_option(menu_option_arr, menu_option, 6);
        print_color("\xB2\xB2\xB2\xB2 SELECTIONNEZ UNE OPTION \xB2\xB2\xB2\xB2", "YELLOW", MIDDLE_WIDTH - 17);

        // Release screen mutex
        ReleaseMutex(hScreenMutex);

        rc.Event.KeyEvent.wVirtualScanCode = 0;
        rc.Event.KeyEvent.wVirtualKeyCode = 0;
        ReadConsoleInputW(hStdin, &rc, 1, &dwRead);

        if (rc.Event.KeyEvent.bKeyDown == FALSE)
        {

            switch (rc.Event.KeyEvent.wVirtualKeyCode)
            {
            case 0x51:
                // menu_is_running = 0;
                break;
            case VK_UP:
                if (menu_option - 1 >= 1)
                {
                    menu_option -= 1;
                }
                else
                {
                    menu_option = 6;
                }
                break;
            case VK_DOWN:
                if (menu_option + 1 <= 6)
                {
                    menu_option += 1;
                }
                else
                {
                    menu_option = 1;
                }
                break;
            case VK_RETURN:
                HANDLE h_tr = (HANDLE)_beginthread(handle_operation, 0, (void *)(uintptr_t)menu_option);
                WaitForSingleObject(h_tr, INFINITE);
                break;
            }
        }
    }
}

void restore_console()
{
    SetConsoleTextAttribute(hStdout, default_text_attributes);
    SetConsoleScreenBufferSize(hStdout, default_console_info.dwSize);
    SetConsoleWindowInfo(hStdout, TRUE, &default_console_info.srWindow);
    // SetConsoleCursorInfo(hStdout, &old_cursor_info);
}