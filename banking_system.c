#define _WIN32_WINNT 0x0500
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

#define PATH_SIZE 50
#define ACCOUNT_NO_SIZE 50
#define PASSWORD_SIZE 50
#define NAME_SIZE 50
#define PHONE_SIZE 50

/* User data structure */
struct customer
{
    char name[50];
    char phone_number[50];
    char account_number[ACCOUNT_NO_SIZE];
    char password[PASSWORD_SIZE];
    float balance;
};

int exit_satus = 0;

void print_spaces(int n)
{
    for (int i = 0; i < n; i++)
        printf(" ");
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

    printf("\n\n-------------------------------");
    printf("\n\nInformation du compte %s", u.account_number);
    printf("\n\nTitulaire du compte: %s", u.name);
    printf("\nNumero de compte: %s", u.account_number);
    printf("\nNumero de telephone: %s", u.phone_number);
    printf("\nSolde: %f", u.balance);
    printf("\n-------------------------------\n\n");
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

void add_customer()
{

    struct customer new_customer;
    char path[PATH_SIZE] = "./client/";
    char temp[ACCOUNT_NO_SIZE];
    FILE *file;

    printf("Indiquer un identifiant:\t");
    scanf("%s", new_customer.account_number);

    printf("Indiquer un nom:\t");
    scanf("%s", new_customer.name);

    printf("Indiquer un numero de telephone:\t");
    scanf("%s", new_customer.phone_number);

    printf("Indiquer un mot de passe:\t");
    scanf("%s", new_customer.password);

    new_customer.balance = 0;

    strcpy(temp, new_customer.account_number);
    strcat(path, strcat(temp, ".dat"));

    if ((file = fopen(path, "w")))
    {
        fwrite(&new_customer, sizeof(struct customer), 1, file);
        if (fwrite != 0)
        {
            printf("\n\nCompte numero %s ajoute!", &new_customer.account_number);
            fclose(file);
            customer_info(new_customer.account_number);
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

void get_date()
{
    time_t now = time(NULL);
    struct tm tm = *localtime(&now);
    printf("\nLe %02d-%02d-%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
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

void menu(struct customer current_customer)
{
    system("cls");
    int columns, columns_by_2;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    columns_by_2 = columns / 2;

    print_spaces(columns_by_2 - 24);
    print_image(columns_by_2 - 24, "title-image.txt");

    int opt;

    printf("\nBienvenue dans votre espace %s", current_customer.name);
    printf("\n\nEffectuer une operation: ");
    printf("\n\n1. Effectuer un Retrait");
    printf("\n2. Consulter mon solde");
    printf("\n3. Effectuer un virement");
    printf("\n4. Afficher mes informations");
    printf("\n5. Mofidier mon mot de passe");
    printf("\n6. Terminer l'operation en cours");

    printf("\n\nSelectionnez une option: \t");
    scanf("%d", &opt);

    switch (opt)
    {
    case 1:
        withdraw(current_customer.account_number);
        break;
    case 2:
        get_balance(current_customer.account_number);
        break;
    case 3:
        break;
    case 4:
        customer_info(current_customer.account_number);
        break;
    case 5:
        break;
    case 6:
        break;
    }
}

void SetConsoleColour(WORD *Attributes, DWORD Colour)
{
    CONSOLE_SCREEN_BUFFER_INFO Info;
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hStdout, &Info);
    *Attributes = Info.wAttributes;
    SetConsoleTextAttribute(hStdout, Colour);
}

void ResetConsoleColour(WORD Attributes)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Attributes);
}

int main()
{
    /* Activer l'accés en lecture/écriture */
    HANDLE writeHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    HANDLE readHandle = GetStdHandle(STD_INPUT_HANDLE);
    WORD Attributes = 0;

    while (exit_satus == 0)
    {
        int opt;
        char account_number[ACCOUNT_NO_SIZE];
        char password[PASSWORD_SIZE];
        int connected = 0;
        int columns, columns_by_2;
        int window_width = 70;
        int window_height = 40;
        int selected_row = 1;

        SetConsoleTitle("C-ATM Management by Marinos");

        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(CONSOLE_FONT_INFOEX);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 0;
        cfi.dwFontSize.Y = 14;
        // cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = 400; // FW_NORMAL
        wcscpy(cfi.FaceName, L"Consolas");
        SetCurrentConsoleFontEx(writeHandle, FALSE, &cfi);

        /* set up the size */
        COORD coord;
        coord.X = window_width;
        coord.Y = window_height;

        SMALL_RECT rect;
        rect.Top = 0;
        rect.Left = 0;
        rect.Bottom = window_height - 1;
        rect.Right = window_width - 1;

        SetConsoleScreenBufferSize(writeHandle, coord);
        SetConsoleWindowInfo(writeHandle, TRUE, &rect);

        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(writeHandle, &csbi);
        columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
        columns_by_2 = columns / 2;

        system("cls");
        system("color 17");
        print_spaces(columns_by_2 - 24);
        print_image(columns_by_2 - 24, "title-image.txt");
        printf("\n\n");

        print_spaces(columns_by_2 - 15);
        printf("\xB2\xB2\xB2\xB2 EFFECTUER UNE OPERATION \xB2\xB2\xB2\xB2\n\n");

        print_spaces(columns_by_2 - 12);

        SetConsoleColour(&Attributes, FOREGROUND_INTENSITY | FOREGROUND_BLUE);
        printf("1. CREER UN COMPTE\n\n");
        ResetConsoleColour(Attributes);

        print_spaces(columns_by_2 - 12);
        printf("2. ACCEDER A MON ESPACE\n\n");

        print_spaces(columns_by_2 - 12);
        printf("3. QUITTER\n\n");

        print_image(columns_by_2 - 26, "footer.txt");
        printf("\n\n");
        print_spaces(columns_by_2 - 14);
        printf("\xDB\xDB\xDB SELECTIONNEZ UNE OPTION \xDB\xDB\xDB ");
        print_spaces(columns_by_2 - 12);
        scanf("%d", &opt);

        if (opt == 1)
        {
            system("cls");
            add_customer();
        }
        else if (opt == 2)
        {
            system("cls");
            printf("\nVeuillez indiquer votre numero de compte: \t");
            scanf("%s", &account_number);

            if (check_account(account_number))
            {
                printf("\nMot de passe: \t");
                scanf("%s", &password);

                if ((check_password(account_number, password)))
                {
                    connected = 1;
                    system("cls");

                    while (connected == 1)
                    {
                        system("cls");
                        struct customer current_customer = get_record(account_number);

                        menu(current_customer);

                        printf("\n\nContinuer la navigation ? [1/0] : \t");
                        scanf("%d", &connected);
                    }
                }
                else
                {
                    system("cls");
                    printf("\nLe mot de passe est incorect!");
                }
            }
            else
            {
                system("cls");
                printf("\nCE COMMPTE N'EXISTE PAS!");
            }
        }
    }
}
