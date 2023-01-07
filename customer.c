#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <process.h>
#include "customer.h"
#include "interface.h"

/* Global user login informations*/
char account_number[ACCOUNT_NO_SIZE];
char password[PASSWORD_SIZE];
int connected = 0;

HANDLE h_ctm_threads[MAX_CUSTOMER_THREADS] = {NULL};
HANDLE menuThread = {NULL};
int customer_threads = 0;

void create_account(void *arg)
{
    loading_screen();

    struct customer new_customer;
    char path[PATH_SIZE] = "./client/";
    char temp[ACCOUNT_NO_SIZE];
    FILE *file;

    // Wait for display to be available, then lock it.
    WaitForSingleObject(hScreenMutex, INFINITE);

    /* Display */
    print_image(40 - 24, "title-image.txt");
    line_break(3);
    print_color("\xB2\xB2\xB2\xB2 AJOUTER UN COMPTE \xB2\xB2\xB2\xB2\n\n", "YELLOW", MIDDLE_WIDTH - 17);
    line_break(3);
    print_color("NUMERO DE COMPTE: \t", "YELLOW", MIDDLE_WIDTH - 12);
    scanf("%s", new_customer.account_number);
    line_break(1);
    print_color("NOM DE FAMILLE: \t", "YELLOW", MIDDLE_WIDTH - 12);
    scanf("%s", new_customer.name);
    line_break(1);
    print_color("NUMERO DE TEL: \t", "YELLOW", MIDDLE_WIDTH - 12);
    scanf("%s", new_customer.phone_number);
    line_break(1);
    print_color("MOT DE PASSE: \t", "YELLOW", MIDDLE_WIDTH - 12);
    scanf("%s", new_customer.password);
    new_customer.balance = 0;
    strcpy(temp, new_customer.account_number);
    strcat(path, strcat(temp, ".dat"));

    // Unlock screen mutex
    ReleaseMutex(hScreenMutex);

    if ((file = fopen(path, "w")))
    {
        fwrite(&new_customer, sizeof(struct customer), 1, file);
        if (fwrite != 0)
        {
            line_break(3);
            print_color("CREATE ACCOUNT SUCCESFULLY!\n", "GREEN", MIDDLE_WIDTH - 15);
            fclose(file);
            customer_info_get(new_customer.account_number);
            line_break(3);
            print_color("Press key to return to menu...\n", "WHITE", MIDDLE_WIDTH - 15);
            getch();
        }
        else
        {
            fprintf(stderr, "\nErreur dans la création du compte\n");
        }
    }
    else
    {
        fprintf(stderr, "\nErreur dans la création du compte\n");
    }
}

void login_account(void *arg)
{
    // Wait for display to be available, then lock it.
    WaitForSingleObject(hScreenMutex, INFINITE);

    system("cls");
    print_image(MIDDLE_WIDTH - 24, "title-image.txt");
    line_break(3);
    print_color("\xB2\xB2\xB2\xB2 SE CONNECTER \xB2\xB2\xB2\xB2", "YELLOW", MIDDLE_WIDTH - 13);
    line_break(2);
    print_color("\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF", "WHITE", MIDDLE_WIDTH - 13);
    line_break(3);
    print_color("NUMERO DE COMPTE: \t", "WHITE", MIDDLE_WIDTH - 13);
    scanf("%s", &account_number);

    if (check_account(account_number))
    {
        line_break(3);
        print_color("MOT DE PASSE: \t", "WHITE", MIDDLE_WIDTH - 13);

        // Unlock screen mutex
        ReleaseMutex(hScreenMutex);

        scanf("%s", &password);

        if ((check_password(account_number, password)))
        {
            connected = 1;
            struct customer current_customer = record_get(account_number);
            menuThread = (HANDLE)_beginthread(menu, 0, (void *)current_customer.name);
            WaitForSingleObject(menuThread, INFINITE);
        }
        else
        {
            line_break(2);
            print_color("MOT DE PASSE INCORRECT!", "RED", MIDDLE_WIDTH - 17);
            line_break(2);
            printf("Appuyer sur une touche pour quitter...");
            getch();
        }
    }
    else
    {
        line_break(2);
        print_color("IDENTIFIANT INCORRECT!", "RED", MIDDLE_WIDTH - 17);
        line_break(2);
        printf("Appuyer sur une touche pour quitter...");
        getch();
    }
}

int check_account(char customer_account_no[ACCOUNT_NO_SIZE])
{
    char temp[ACCOUNT_NO_SIZE];
    char path[PATH_SIZE] = "./client/";
    strcpy(temp, customer_account_no);
    strcat(path, strcat(temp, ".dat"));
    FILE *file;

    if ((file = fopen(path, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int check_password(char customer_account_no[ACCOUNT_NO_SIZE], char password[PASSWORD_SIZE])
{
    char temp[ACCOUNT_NO_SIZE];
    struct customer u;
    char path[PATH_SIZE] = "./client/";
    FILE *file;

    strcpy(temp, customer_account_no);
    strcat(path, strcat(temp, ".dat"));

    if ((file = fopen(path, "r")))
    {
        fread(&u, sizeof(struct customer), 1, file);
        fclose(file);

        if (!strcmp(password, u.password))
        {
            return 1;
        }
        return 0;
    }
    return 0;
}

// EFFECTUER UN RETRAIT", "CONSULTER MON SOLDE", "EFFECTUER UN VIREMENT", "AFFICHER MES INFORMATIONS", "MODIFIER MON MOT DE PASSE", "TERMINER L'OPERATION EN COURS
void handle_operation(void *selector)
{
    rc.Event.KeyEvent.wVirtualKeyCode = VK_OEM_CLEAR;
    int *select = (void *)&selector;
    switch (*select)
    {
    case 1:
        ++customer_threads;
        h_ctm_threads[customer_threads] = (HANDLE)_beginthread(withdraw_process, 0, (void *)account_number);
        WaitForSingleObject(h_ctm_threads[customer_threads], INFINITE);
        --customer_threads;
        ResumeThread(menuThread);
        break;
    case 2:
        ++customer_threads;
        h_ctm_threads[customer_threads] = (HANDLE)_beginthread(customer_info_get, 0, (void *)account_number);
        WaitForSingleObject(h_ctm_threads[customer_threads], INFINITE);
        --customer_threads;
        break;
    case 3:
        ++customer_threads;
        h_ctm_threads[customer_threads] = (HANDLE)_beginthread(customer_info_get, 0, (void *)account_number);
        WaitForSingleObject(h_ctm_threads[customer_threads], INFINITE);
        --customer_threads;
        break;
    case 4:
        ++customer_threads;
        h_ctm_threads[customer_threads] = (HANDLE)_beginthread(withdraw_process, 0, (void *)account_number);
        WaitForSingleObject(h_ctm_threads[customer_threads], INFINITE);
        --customer_threads;
        break;
    case 5:
        ++customer_threads;
        h_ctm_threads[customer_threads] = (HANDLE)_beginthread(withdraw_process, 0, (void *)account_number);
        WaitForSingleObject(h_ctm_threads[customer_threads], INFINITE);
        --customer_threads;
        break;
    case 6:
        // app_shutdown();
        break;
    }
}

void withdraw_process(void *customer_account_no)
{
    /* get up-to-date recorded information */
    char temp[ACCOUNT_NO_SIZE];
    struct customer u;
    char path[PATH_SIZE] = "./client/";
    float amount = 0;
    FILE *file;

    strcpy(temp, customer_account_no);
    strcat(path, strcat(temp, ".dat"));

    if ((file = fopen(path, "r")))
    {
        fread(&u, sizeof(struct customer), 1, file);
        fclose(file);
    }

    // Display
    system("cls");
    print_image(MIDDLE_WIDTH - 24, "title-image.txt");
    line_break(3);
    print_color("\xB2\xB2\xB2\xB2 EFFECTUER UN RETRAIT \xB2\xB2\xB2\xB2", "YELLOW", MIDDLE_WIDTH - 17);
    line_break(2);
    print_color("MONTANT DU RETRAIT: \t", "YELLOW", MIDDLE_WIDTH - 17);
    scanf("%f", &amount);
    u.balance -= amount;

    // Update recorded information
    if ((file = fopen(path, "w")))
    {
        fwrite(&u, sizeof(struct customer), 1, file);
        if (fwrite != NULL)
        {
            fclose(file);
            system("cls");
            print_image(MIDDLE_WIDTH - 24, "title-image.txt");
            line_break(3);
            print_color("RETRAIT EFFECTUE AVEC SUCCES", "GREEN", MIDDLE_WIDTH - 17);
            line_break(2);
            customer_info_get(u.account_number);
        }
        else
        {
            fprintf(stderr, "\nRetrait impossible\n");
        }
    }
}

void customer_info_get(void *customer_account_no)
{
    char temp[ACCOUNT_NO_SIZE];
    struct customer u;
    char path[PATH_SIZE] = "./client/";
    FILE *file;

    strcpy(temp, customer_account_no);
    strcat(path, strcat(temp, ".dat"));

    if ((file = fopen(path, "r")))
    {
        fread(&u, sizeof(struct customer), 1, file);
        fclose(file);
    }

    line_break(3);
    print_color("~~~~~~~~~~ ACCOUNT INFORMATION ~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 20);
    line_break(2);
    print_spaces(MIDDLE_WIDTH - 20);
    printf("ACCOUNT NUMBER: \t\t %s", u.account_number);
    line_break(2);
    print_spaces(MIDDLE_WIDTH - 20);
    printf("ACCOUNT OWNER: \t\t %s", u.name);
    line_break(2);
    print_spaces(MIDDLE_WIDTH - 20);
    printf("PHONE NUMBER: \t\t %s", u.phone_number);
    line_break(2);
    print_spaces(MIDDLE_WIDTH - 20);
    printf("BALANCE: \t\t\t %f", u.balance);
    line_break(2);
    print_color("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 20);
    line_break(2);
    print_color("Press key to continue...", "WHITE", MIDDLE_WIDTH);
    _getch();
}

struct customer record_get(char customer_account_no[ACCOUNT_NO_SIZE])
{
    char temp[ACCOUNT_NO_SIZE];
    struct customer u;
    char path[PATH_SIZE] = "./client/";
    FILE *file;

    strcpy(temp, customer_account_no);
    strcat(path, strcat(temp, ".dat"));

    if ((file = fopen(path, "r")))
    {
        fread(&u, sizeof(struct customer), 1, file);
        fclose(file);
        return u;
    }
    else
    {
        fprintf(stderr, "Donnee indisponible\n");
        exit(-1);
    }
}

int balance_get(void *customer_account_no)
{
    /* get up-to-date recorded information */
    char temp[ACCOUNT_NO_SIZE];
    struct customer u;
    char path[PATH_SIZE] = "./client/";
    FILE *file;

    strcpy(temp, customer_account_no);
    strcat(path, strcat(temp, ".dat"));

    if ((file = fopen(path, "r")))
    {
        fread(&u, sizeof(struct customer), 1, file);
        fclose(file);
        printf("\nVotre solde: %f", u.balance);
        return 1;
    }
    else
    {
        fprintf(stderr, "Solde indisponible\n");
        return 0;
    }
}
