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

    char main_opt[3][50] = {"CREER UN COMPTE", "ACCEDER A MON ESPACE", "QUITTER"};
    int opt = 1;

    // Setup console
    HANDLE h_setup = (HANDLE)_beginthread(setup_console, 0, NULL);
    while (is_setup == FALSE)
    {
        loading();
    }
    WaitForSingleObject(h_setup, INFINITE);
    CloseHandle(h_setup);

    // Create the mutexes and reset thread count.
    hScreenMutex = CreateMutexW(NULL, FALSE, NULL);
    hRunMutex = CreateMutexW(NULL, TRUE, NULL);

    while (exit_status == 0)
    {

        // Wait for display to be available, then lock it.
        WaitForSingleObject(hScreenMutex, INFINITE);

        // Display text iànterface
        system("cls");
        print_image(MIDDLE_WIDTH - 24, "title-image.txt");
        line_break(2);
        print_color("\xB0\xB0\xB0\xB0\xB0\xB0  EFFECTUER UNE OPERATION  \xB0\xB0\xB0\xB0\xB0\xB0", "YELLOW", MIDDLE_WIDTH - 20);
        line_break(3);
        list_option(main_opt, opt, 3);
        line_break(4);
        print_color("Press ENTER to select...", "WHITE", MIDDLE_WIDTH);

        // Release
        ReleaseMutex(hScreenMutex);

        // Event Handling
        ReadConsoleInputW(hStdin, &rc, 1, &dwRead);

        if (rc.Event.KeyEvent.bKeyDown == TRUE)
        {
            switch (rc.Event.KeyEvent.wVirtualKeyCode)
            {
            case 0x51:
                app_shutdown();
                break;
            case VK_UP:
                if (opt - 1 >= 1)
                    opt -= 1;
                else
                    opt = 3;
                break;
            case VK_DOWN:
                if (opt + 1 <= 3)
                    opt += 1;
                else
                    opt = 1;
                break;
            case VK_RETURN:
                handle_selection(opt);
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
    exit_status = 1;
}
