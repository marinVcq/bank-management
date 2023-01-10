#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>
#include <process.h>
#include "interface.h"
#include "customer.h"

// Define global variables
CONSOLE_SCREEN_BUFFER_INFO default_console_info = {0};
WORD default_text_attributes = 0;
DWORD dwRead = 0;
HANDLE hStdout = NULL;
HANDLE hStdin = NULL;
INPUT_RECORD rc = {0};
CONSOLE_CURSOR_INFO cursor_info = {0};
CONSOLE_FONT_INFOEX cfi = {0};
DWORD dwOutMode = 0;
DWORD dwInMode = 0;
BOOL is_setup = FALSE;

void setup_console(void *arg)
{
    // Define handle
    if ((hStdout = GetStdHandle(STD_OUTPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        printf("Error in GetStdHandle(STD_OUTPUT_HANDLE): %d\n", GetLastError());
    if ((hStdin = GetStdHandle(STD_INPUT_HANDLE)) == INVALID_HANDLE_VALUE)
        printf("Error in GetStdHandle(STD_INPUT_HANDLE): %d\n", GetLastError());

    // Save default parameters
    if (GetConsoleScreenBufferInfo(hStdout, &default_console_info) == 0)
        printf("Error in GetConsoleScreenBufferInfo: %d\n", GetLastError());
    default_text_attributes = default_console_info.wAttributes;

    // Set console title
    SetConsoleTitle("C-ATM Management by Marin");

    // Set console size
    COORD console_size;
    console_size.X = CONSOLE_WIDTH;
    console_size.Y = CONSOLE_HEIGHT;

    SMALL_RECT rect;
    rect.Top = 0;
    rect.Left = 0;
    rect.Bottom = CONSOLE_HEIGHT - 1;
    rect.Right = CONSOLE_WIDTH - 1;

    // Set console's window & buffer size
    if (SetConsoleWindowInfo(hStdout, TRUE, &rect) == 0)
        printf("Error in SetConsoleWindowInfo: %d\n", GetLastError());
    if (SetConsoleScreenBufferSize(hStdout, console_size) == 0)
        printf("Error in SetConsoleScreenBufferSize: %d\n", GetLastError());

    // Set console font
    cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 15;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = 400; // FW_NORMAL
    wcscpy(cfi.FaceName, L"Consolas");
    if (SetCurrentConsoleFontEx(hStdout, FALSE, &cfi) == 0)
        printf("Error in SetCurrentConsoleFontEx: %d\n", GetLastError());

    // Set input and output mode
    GetConsoleMode(hStdout, &dwOutMode);
    GetConsoleMode(hStdin, &dwInMode);
    SetConsoleMode(hStdout, dwOutMode);
    SetConsoleMode(hStdin, dwInMode);

    // Hide cursor
    if (GetConsoleCursorInfo(hStdout, &cursor_info) == 0)
        printf("Error in SetConsoleCursorInfo: %d\n", GetLastError());
    cursor_info.bVisible = FALSE;
    if (SetConsoleCursorInfo(hStdout, &cursor_info) == 0)
        printf("Error in SetConsoleCursorInfo: %d\n", GetLastError());

    // Sleep a little bit and terminate
    Sleep(1000);
    is_setup = TRUE;
    _endthread();
}

void loading()
{
    // Wait for display to be available, then lock it.
    WaitForSingleObject(hScreenMutex, INFINITE);

    system("cls");
    print_image(MIDDLE_WIDTH - 24, "title-image.txt");
    line_break(3);
    print_color("Loading", "YELLOW", MIDDLE_WIDTH - 7);

    for (int i = 0; i < 4; i++)
    {
        fordelay(300000000);
        print_color(".", "YELLOW", 0);
        if (i >= 4)
        {
            system("cls");
            i = 0;
        }
    }

    // Release
    ReleaseMutex(hScreenMutex);
}

void print_date(void)
{
    time_t now = time(NULL);
    struct tm tm = *localtime(&now);
    printf("\nLe %02d-%02d-%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

void fordelay(long long int j)
{
    long long int i, k;
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

void list_option(char opt_arr[][50], int selector, int size)
{

    for (int i = 0; i < size; i++)
    {
        if ((i + 1) == selector)
        {
            print_spaces(MIDDLE_WIDTH - 14);
            SetConsoleTextAttribute(hStdout, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
            printf("\xAF %d. %s\n\n", (i + 1), opt_arr[i]);
            SetConsoleTextAttribute(hStdout, default_text_attributes);
        }
        else
        {
            print_spaces(MIDDLE_WIDTH - 14);
            printf("%d. %s\n\n", (i + 1), opt_arr[i]);
        }
    }
}

void menu(void *name)
{
    int opt = 1;
    char opt_arr[6][50] = {"EFFECTUER UN RETRAIT", "CONSULTER MON SOLDE", "EFFECTUER UN VIREMENT", "AFFICHER MES INFORMATIONS", "MODIFIER MON MOT DE PASSE", "TERMINER L'OPERATION EN COURS"};
    BOOL Continue = TRUE;

    while (Continue)
    {

        // Wait for display to be available, then lock it.
        WaitForSingleObject(hScreenMutex, INFINITE);

        system("cls");
        print_image(MIDDLE_WIDTH - 24, "title-image.txt");
        line_break(2);
        print_color("\xB0\xB0\xB0\xB0\xB0\xB0  EFFECTUER UNE OPERATION  \xB0\xB0\xB0\xB0\xB0\xB0", "YELLOW", MIDDLE_WIDTH - 20);
        line_break(3);
        list_option(opt_arr, opt, 6);
        line_break(3);
        print_color("Press ENTER to select...", "WHITE", MIDDLE_WIDTH);

        // Release screen mutex
        ReleaseMutex(hScreenMutex);

        // Listen for keyboard events
        ReadConsoleInputW(hStdin, &rc, 1, &dwRead);

        if (rc.Event.KeyEvent.bKeyDown == TRUE)
        {
            switch (rc.Event.KeyEvent.wVirtualKeyCode)
            {
            case 0x51:
                // menu_is_running = 0;
                break;
            case VK_UP:
                if (opt - 1 >= 1)
                {
                    opt -= 1;
                }
                else
                {
                    opt = 6;
                }
                break;
            case VK_DOWN:
                if (opt + 1 <= 6)
                {
                    opt += 1;
                }
                else
                {
                    opt = 1;
                }
                break;
            case VK_RETURN:
                HANDLE h_tr = (HANDLE)_beginthread(handle_operation, 0, (void *)(uintptr_t)opt);
                WaitForSingleObject(h_tr, INFINITE);
                break;
            }
        }
    }
}

void restore_console(void)
{
    SetConsoleTextAttribute(hStdout, default_text_attributes);
    SetConsoleScreenBufferSize(hStdout, default_console_info.dwSize);
    SetConsoleWindowInfo(hStdout, TRUE, &default_console_info.srWindow);
}

void print_color(char *string, char *color, int margin)
{
    CONSOLE_SCREEN_BUFFER_INFO temp;

    // Store default text attributes
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

    // Reset text attributes
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), temp.wAttributes);
}

void line_break(int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("\n");
    }
}

void print_spaces(int n)
{
    for (int i = 0; i < n; i++)
        printf(" ");
}