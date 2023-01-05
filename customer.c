#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "customer.h"
#include "interface.h"

/* Global user login informations*/
char account_number[ACCOUNT_NO_SIZE];
char password[PASSWORD_SIZE];
int connected = 0;

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

void customer_info(char customer_account_no[ACCOUNT_NO_SIZE])
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

    line_break(2);
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
    printf("BALANCE: \t\t %d", u.balance);
    line_break(2);
    print_color("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~", "YELLOW", MIDDLE_WIDTH - 20);
}

struct customer get_record(char customer_account_no[ACCOUNT_NO_SIZE])
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

void create_account(void)
{
    loading_screen();

    struct customer new_customer;
    char path[PATH_SIZE] = "./client/";
    char temp[ACCOUNT_NO_SIZE];
    FILE *file;

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

    if ((file = fopen(path, "w")))
    {
        fwrite(&new_customer, sizeof(struct customer), 1, file);
        if (fwrite != 0)
        {
            line_break(3);
            print_color("CREATE ACCOUNT SUCCESFULLY!\n", "GREEN", MIDDLE_WIDTH - 15);
            fclose(file);
            customer_info(new_customer.account_number);
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

int withdraw(char customer_account_no[ACCOUNT_NO_SIZE])
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
    }

    float amount;
    printf("\n\nMontant du retrait: \t");
    scanf("%f", &amount);
    u.balance -= amount;

    /* Update recorded information */
    if ((file = fopen(path, "w")))
    {
        fwrite(&u, sizeof(struct customer), 1, file);
        if (fwrite != NULL)
        {
            fclose(file);
            return 1;
        }
        else
        {
            fprintf(stderr, "\nRetrait impossible\n");
            return 0;
        }
    }
    return 0;
}

int get_balance(char customer_account_no[ACCOUNT_NO_SIZE])
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

void login_account(void)
{

    loading_screen();

    int columns_by_2 = MIDDLE_WIDTH;

    /* Display banner */
    print_spaces(columns_by_2 - 24);
    print_image(columns_by_2 - 24, "title-image.txt");
    printf("\n\n");

    /* Formulaire */
    print_spaces(columns_by_2 - 24);
    printf("NUMERO DE COMPTE: \t");
    scanf("%s", &account_number);

    if (check_account(account_number))
    {
        printf(DOUBLE_JUMP);
        print_spaces(columns_by_2 - 24);
        printf("MOT DE PASSE: \t");
        scanf("%s", &password);

        if ((check_password(account_number, password)))
        {
            connected = 1;

            while (connected == 1)
            {
                struct customer current_customer = get_record(account_number);

                menu(current_customer.name);

                printf("\n\nContinuer la navigation ? [1/0] : \t");
                scanf("%d", &connected);
            }
        }
        else
        {
            printf(DOUBLE_JUMP);
            print_spaces(columns_by_2 - 24);
            printf("MOT DE PASSE INCORRECT!");
            printf(DOUBLE_JUMP);
            print_spaces(columns_by_2 - 24);
            printf("Appuyer sur une touche pour quitter...");
            getch();
        }
    }
    else
    {
        printf(DOUBLE_JUMP);
        print_spaces(columns_by_2 - 24);
        printf("NUMERE DE COMPTE INEXISTANT OU INCORRECT!");
        printf(DOUBLE_JUMP);
        print_spaces(columns_by_2 - 24);
        printf("Appuyer sur une touche pour quitter...");
        getch();
    }
}