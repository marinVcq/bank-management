#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <process.h>
#include <time.h>
#include "customer.h"
#include "interface.h"

#define MAX_THREADS 3

HANDLE hRunMutex;                      // "Keep Running" mutex
HANDLE hScreenMutex;                   // "Screen update" mutex
HANDLE hThreads[MAX_THREADS] = {NULL}; // Handles for created threads
int ThreadNr = 0;                      // Number of threads started

int exit_status = 0;
void handle_selection(int selector);
void app_shutdown(void);

int main()
{

    loading_screen();

    char main_option_arr[3][50] = {"CREER UN COMPTE", "ACCEDER A MON ESPACE", "QUITTER"};
    int selected_option = 1;

    // Setup console
    setup_console();

    // Create the mutexes and reset thread count.
    hScreenMutex = CreateMutexW(NULL, FALSE, NULL); // Cleared
    hRunMutex = CreateMutexW(NULL, TRUE, NULL);     // Set

    while (exit_status == 0)
    {

        // Wait for display to be available, then lock it.
        WaitForSingleObject(hScreenMutex, INFINITE);

        // Display Interface
        system("cls");
        print_image(40 - 24, "title-image.txt");
        line_break(4);
        print_color("\xB2\xB2\xB2\xB2 EFFECTUER UNE OPERATION \xB2\xB2\xB2\xB2\n\n", "YELLOW", MIDDLE_WIDTH - 17);
        line_break(2);
        list_option(main_option_arr, selected_option, 3);
        line_break(2);
        print_color("\xB2\xB2\xB2\xB2 PRESS ENTER TO SELECT \xB2\xB2\xB2\xB2\n\n", "YELLOW", MIDDLE_WIDTH - 16);

        // Release
        ReleaseMutex(hScreenMutex);

        // Event Handling
        ReadConsoleInputW(hStdin, &rc, 1, &dwRead);

        if (rc.Event.KeyEvent.bKeyDown == FALSE)
        {
            switch (rc.Event.KeyEvent.wVirtualKeyCode)
            {
            case 0x51:
                app_shutdown();
                break;
            case VK_UP:
                if (selected_option - 1 >= 1)
                    selected_option -= 1;
                else
                    selected_option = 3;
                break;
            case VK_DOWN:
                if (selected_option + 1 <= 3)
                    selected_option += 1;
                else
                    selected_option = 1;
                break;
            case VK_RETURN:
                handle_selection(selected_option);
                break;
            }
        }
    }

    // All threads done. Clean up handles.
    if (hScreenMutex)
        CloseHandle(hScreenMutex);
    if (hRunMutex)
        CloseHandle(hRunMutex);
    if (hStdout)
        CloseHandle(hStdout);
    if (hStdin)
        CloseHandle(hStdin);

    // Restore parameters
    restore_console();
    system("cls");
}

void handle_selection(int selector)
{
    switch (selector)
    {
    case 1:
        ++ThreadNr;
        hThreads[ThreadNr] = (HANDLE)_beginthread(create_account, 0, (void *)(uintptr_t)ThreadNr);
        WaitForSingleObject(hThreads[ThreadNr], INFINITE);
        --ThreadNr;
        break;
    case 2:
        ++ThreadNr;
        hThreads[ThreadNr] = (HANDLE)_beginthread(login_account, 0, (void *)(uintptr_t)ThreadNr);
        WaitForSingleObject(hThreads[ThreadNr], INFINITE);
        --ThreadNr;
        break;
    case 3:
        app_shutdown();
        break;
    }
}

void app_shutdown()
{
    WaitForMultipleObjects(ThreadNr, hThreads, TRUE, INFINITE);
    exit_status = 1;
    for (int i = 0; i < ThreadNr; i++)
    {
        if (hThreads[i])
            CloseHandle(hThreads[i]);
    }
    if (hScreenMutex)
        CloseHandle(hScreenMutex);
    if (hRunMutex)
        CloseHandle(hRunMutex);

    system("cls");
}
