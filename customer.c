#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
    struct customer new_customer;
    char customer_label[4][25] = {"NUMERO DE COMPTE:", "NOM DE FAMILLE:", "NUMERO DE TEL:", "MOT DE PASSE:"};
    char path[PATH_SIZE] = "./client/";
    char temp[ACCOUNT_NO_SIZE];
    FILE *file;

    // Wait for display to be available, then lock it.
    WaitForSingleObject(hScreenMutex, INFINITE);

    /* Display */
    system("cls");
    print_image(MIDDLE_WIDTH - 24, "title-image.txt");
    line_break(2);
    print_color("\xB0\xB0\xB0\xB0\xB0\xB0  AJOUTER UN COMPTE  \xB0\xB0\xB0\xB0\xB0\xB0", "YELLOW", MIDDLE_WIDTH - 18);
    line_break(3);
    print_color("NUMERO DE COMPTE: \t", "YELLOW", MIDDLE_WIDTH - 13);
    scanf("%s", new_customer.account_number);
    line_break(1);
    print_color("NOM DE FAMILLE: \t", "YELLOW", MIDDLE_WIDTH - 13);
    scanf("%s", new_customer.name);
    line_break(1);
    print_color("NUMERO DE TEL: \t", "YELLOW", MIDDLE_WIDTH - 13);
    scanf("%s", new_customer.phone_number);
    line_break(1);
    print_color("MOT DE PASSE: \t", "YELLOW", MIDDLE_WIDTH - 13);
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
            // Wait for display to be available, then lock it.
            WaitForSingleObject(hScreenMutex, INFINITE);

            system("cls");
            print_image(MIDDLE_WIDTH - 24, "title-image.txt");
            line_break(2);
            print_color("\xB0\xB0\xB0\xB0\xB0\xB0  CREATE ACCOUNT SUCCESFULLY  \xB0\xB0\xB0\xB0\xB0\xB0", "GREEN", MIDDLE_WIDTH - 21);
            line_break(2);
            print_color("~~~~~~~~~~ ACCOUNT INFORMATION ~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 21);
            line_break(2);
            print_spaces(MIDDLE_WIDTH - 21);
            printf("ACCOUNT NUMBER: \t\t %s", new_customer.account_number);
            line_break(2);
            print_spaces(MIDDLE_WIDTH - 21);
            printf("ACCOUNT OWNER: \t\t %s", new_customer.name);
            line_break(2);
            print_spaces(MIDDLE_WIDTH - 21);
            printf("PHONE NUMBER: \t\t %s", new_customer.phone_number);
            line_break(2);
            print_spaces(MIDDLE_WIDTH - 21);
            printf("BALANCE: \t\t\t %f", new_customer.balance);
            line_break(2);
            print_color("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 21);
            line_break(2);
            print_color("Press key to continue...", "WHITE", MIDDLE_WIDTH);
            _getch();
            fclose(file);

            // Unlock screen mutex & terminate
            ReleaseMutex(hScreenMutex);
            _endthread();
        }
        else
        {
            // Wait for display to be available, then lock it.
            WaitForSingleObject(hScreenMutex, INFINITE);

            line_break(3);
            print_color("AN ERROR OCCURS!", "RED", MIDDLE_WIDTH - 12);
            line_break(3);
            print_color("Press key to go back...", "WHITE", MIDDLE_WIDTH);
            // Unlock screen mutex & terminate
            ReleaseMutex(hScreenMutex);
            _getch();
            _endthread();
        }
    }
    else
    {
        // Wait for display to be available, then lock it.
        WaitForSingleObject(hScreenMutex, INFINITE);

        line_break(3);
        print_color("AN ERROR OCCURS!", "RED", MIDDLE_WIDTH - 12);
        line_break(3);
        print_color("Press key to go back...", "WHITE", MIDDLE_WIDTH);

        // Unlock screen mutex & terminate
        ReleaseMutex(hScreenMutex);
        _getch();
        _endthread();
    }
}

void login_account(void *arg)
{
    // Wait for display to be available, then lock it.
    WaitForSingleObject(hScreenMutex, INFINITE);

    system("cls");
    print_image(MIDDLE_WIDTH - 24, "title-image.txt");
    line_break(2);
    print_color("\xB0\xB0\xB0\xB0\xB0\xB0  SE CONNECTER  \xB0\xB0\xB0\xB0\xB0\xB0", "YELLOW", MIDDLE_WIDTH - 14);
    line_break(3);
    print_color("NUMERO DE COMPTE: \t", "WHITE", MIDDLE_WIDTH - 14);
    scanf("%s", &account_number);

    if (check_account(account_number))
    {
        line_break(2);
        print_color("MOT DE PASSE: \t", "WHITE", MIDDLE_WIDTH - 14);

        // Unlock screen mutex
        ReleaseMutex(hScreenMutex);

        // scanf("%s", &password);

        int inc = 0;
        char a;
        BOOL Continue = 1;

        while (Continue)
        {
            a = getch();
            if (a == 13)
            {
                Continue = 0;
                password[inc] = 0;
            }
            else
            {
                password[inc] = a;
                printf("*");
                inc++;
            }
        };

        if ((check_password(account_number, password)))
        {
            connected = 1;
            struct customer current_customer = record_get(account_number);
            menuThread = (HANDLE)_beginthread(menu, 0, (void *)current_customer.name);
            WaitForSingleObject(menuThread, INFINITE);
        }
        else
        {
            line_break(3);
            print_color("INCORRECT PASSWORD!", "RED", MIDDLE_WIDTH - 10);
            line_break(3);
            print_color("Press ENTER to go back...", "WHITE", MIDDLE_WIDTH);
            _getch();
        }
    }
    else
    {
        line_break(3);
        print_color("INCORECT ACCOUNT NUMBER!", "RED", MIDDLE_WIDTH - 12);
        line_break(3);
        print_color("Press ENTER to go back...", "WHITE", MIDDLE_WIDTH);
        _getch();
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
        break;
    case 2:
        ++customer_threads;
        h_ctm_threads[customer_threads] = (HANDLE)_beginthread(balance_get, 0, (void *)account_number);
        WaitForSingleObject(h_ctm_threads[customer_threads], INFINITE);
        --customer_threads;
        break;
    case 3:
        ++customer_threads;
        h_ctm_threads[customer_threads] = (HANDLE)_beginthread(transfer_process, 0, (void *)account_number);
        WaitForSingleObject(h_ctm_threads[customer_threads], INFINITE);
        --customer_threads;
        break;
    case 4:
        ++customer_threads;
        h_ctm_threads[customer_threads] = (HANDLE)_beginthread(customer_info_get, 0, (void *)account_number);
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
    // Get up to date information
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

    // Display text iànterface
    system("cls");
    print_image(MIDDLE_WIDTH - 24, "title-image.txt");
    line_break(2);
    print_color("\xB0\xB0\xB0\xB0\xB0\xB0  EFFECTUER UN RETRAIT  \xB0\xB0\xB0\xB0\xB0\xB0", "YELLOW", MIDDLE_WIDTH - 18);
    line_break(3);
    print_color("MONTANT DU RETRAIT: \t", "WHITE", MIDDLE_WIDTH - 17);
    scanf("%f", &amount);

    // Check for balance
    if (u.balance >= amount)
    {
        u.balance -= amount;

        // Update recorded information
        if ((file = fopen(path, "w")))
        {
            fwrite(&u, sizeof(struct customer), 1, file);
            if (fwrite != NULL)
            {
                fclose(file);
                line_break(3);
                print_color("RETRAIT EFFECTUE AVEC SUCCES", "GREEN", MIDDLE_WIDTH - 17);
                line_break(2);

                // Display information
                print_color("~~~~~~~~~~ INFORMATION ~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 18);
                line_break(2);
                print_spaces(MIDDLE_WIDTH - 18);
                printf("ACCOUNT NUMBER: \t\t %s", u.account_number);
                line_break(2);
                print_spaces(MIDDLE_WIDTH - 18);
                printf("ACCOUNT OWNER: \t\t %s", u.name);
                line_break(2);
                print_spaces(MIDDLE_WIDTH - 18);
                printf("BALANCE: \t\t\t %f", u.balance);
                line_break(2);
                print_color("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 18);
                line_break(2);
                print_color("Press key to continue...", "WHITE", MIDDLE_WIDTH);
                _getch();
            }
        }
    }
    else
    {
        line_break(3);
        print_color("VOS CAPACITE DE RETRAIT SONT INSUFISANTES", "RED", MIDDLE_WIDTH - 21);
        line_break(3);
        print_color("~~~~~~~~~~ INFORMATION ~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 18);
        line_break(2);
        print_spaces(MIDDLE_WIDTH - 18);
        printf("ACCOUNT NUMBER: \t\t %s", u.account_number);
        line_break(2);
        print_spaces(MIDDLE_WIDTH - 18);
        printf("ACCOUNT OWNER: \t\t %s", u.name);
        line_break(2);
        print_spaces(MIDDLE_WIDTH - 18);
        printf("BALANCE: \t\t\t %f", u.balance);
        line_break(2);
        print_color("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 18);
        line_break(2);
        print_color("Press key to continue...", "WHITE", MIDDLE_WIDTH);
        _getch();
    }
}

void transfer_process(void *customer_account_no)
{
    // Get up to date information
    char temp[ACCOUNT_NO_SIZE];
    struct customer u;
    struct customer receiver;
    char path[PATH_SIZE] = "./client/";
    float amount = 0;
    FILE *file;
    char dest[ACCOUNT_NO_SIZE];

    // get issuer's information
    strcpy(temp, customer_account_no);
    strcat(path, strcat(temp, ".dat"));

    if ((file = fopen(path, "r")))
    {
        fread(&u, sizeof(struct customer), 1, file);
        fclose(file);
    }

    system("cls");
    print_image(MIDDLE_WIDTH - 24, "title-image.txt");
    line_break(2);
    print_color("\xB0\xB0\xB0\xB0\xB0\xB0  EFFECTUER UN VIREMENT  \xB0\xB0\xB0\xB0\xB0\xB0", "YELLOW", MIDDLE_WIDTH - 18);
    line_break(3);
    print_color("MONTANT DU VIREMENT: \t", "WHITE", MIDDLE_WIDTH - 17);
    scanf("%f", &amount);
    line_break(2);
    print_color("NUMERO DE COMPTE DESTINATAIRE: \t", "WHITE", MIDDLE_WIDTH - 17);
    scanf("%s", &dest);

    // Check for issuer's transfer capabilities
    if (amount > u.balance)
    {
        line_break(3);
        print_color("VOS CAPACITE DE RETRAIT SONT INSUFISANTES", "RED", MIDDLE_WIDTH - 21);
        line_break(3);
        print_color("~~~~~~~~~~ INFORMATION ~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 18);
        line_break(2);
        print_spaces(MIDDLE_WIDTH - 18);
        printf("ACCOUNT NUMBER: \t\t %s", u.account_number);
        line_break(2);
        print_spaces(MIDDLE_WIDTH - 18);
        printf("ACCOUNT OWNER: \t\t %s", u.name);
        line_break(2);
        print_spaces(MIDDLE_WIDTH - 18);
        printf("BALANCE: \t\t\t %f", u.balance);
        line_break(2);
        print_color("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 18);
        line_break(2);
        print_color("Press key to continue...", "WHITE", MIDDLE_WIDTH);
        _getch();
    }
    else
    {
        // Get receiver account information

        strcpy(temp, "");
        strcpy(path, "");
        strcpy(path, "./client/");
        strcpy(temp, dest);
        strcat(path, strcat(temp, ".dat"));

        printf("%s", path);
        getch();

        if ((file = fopen(path, "r")))
        {
            fread(&receiver, sizeof(struct customer), 1, file);
            fclose(file);

            // Do the transfer update information for both accounts
            receiver.balance += amount;
            u.balance -= amount;

            if (update_information(receiver) && update_information(u))
            {
                line_break(3);
                print_color("VIREMENT EFFECTUE AVEC SUCCES", "GREEN", MIDDLE_WIDTH - 17);
                line_break(2);
                print_color("Press key to continue...", "WHITE", MIDDLE_WIDTH);
                _getch();
            }
            else
            {
                line_break(3);
                print_color("UNE ERREURE EST SURVENUE", "RED", MIDDLE_WIDTH - 21);
                line_break(3);
                print_color("Press key to continue...", "WHITE", MIDDLE_WIDTH);
                _getch();
            }
        }
        else
        {
            line_break(3);
            print_color("LE COMPTE DESTINATAIRE N'EXISTE PAS", "RED", MIDDLE_WIDTH - 21);
            line_break(3);
            print_color("Press key to continue...", "WHITE", MIDDLE_WIDTH);
            _getch();
        }
    }
}

BOOL update_information(struct customer customer)
{

    char temp[ACCOUNT_NO_SIZE];
    char path[PATH_SIZE] = "./client/";
    FILE *file;

    strcpy(temp, "");
    strcpy(temp, customer.account_number);
    strcat(path, strcat(temp, ".dat"));

    if ((file = fopen(path, "w")))
    {
        fwrite(&customer, sizeof(struct customer), 1, file);
        if (fwrite != NULL)
        {
            fclose(file);
            return TRUE;
        }
        else
            return FALSE;
    }
    return FALSE;
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

    system("cls");
    print_image(MIDDLE_WIDTH - 24, "title-image.txt");
    line_break(2);
    print_color("~~~~~~~~~~ ACCOUNT INFORMATION ~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 21);
    line_break(2);
    print_spaces(MIDDLE_WIDTH - 21);
    printf("ACCOUNT NUMBER: \t\t %s", u.account_number);
    line_break(2);
    print_spaces(MIDDLE_WIDTH - 21);
    printf("ACCOUNT OWNER: \t\t %s", u.name);
    line_break(2);
    print_spaces(MIDDLE_WIDTH - 21);
    printf("PHONE NUMBER: \t\t %s", u.phone_number);
    line_break(2);
    print_spaces(MIDDLE_WIDTH - 21);
    printf("BALANCE: \t\t\t %f", u.balance);
    line_break(2);
    print_color("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 21);
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

void balance_get(void *customer_account_no)
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

        // Display
        system("cls");
        print_image(MIDDLE_WIDTH - 24, "title-image.txt");
        line_break(2);
        print_color("~~~~~~~~~~ CONSULTER MON SOLDE ~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 22);
        line_break(2);
        print_spaces(MIDDLE_WIDTH - 22);
        printf("ACCOUNT OWNER: \t\t %s", u.name);
        line_break(2);
        print_spaces(MIDDLE_WIDTH - 22);
        printf("BALANCE: \t\t\t %f", u.balance);
        line_break(2);
        print_color("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 22);
        line_break(3);
        print_color("Press key to continue...", "WHITE", MIDDLE_WIDTH);
        _getch();
    }
    else
        fprintf(stderr, "Solde indisponible\n");
}
