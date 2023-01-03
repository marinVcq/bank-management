#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define FILENAME_SIZE 50
#define ID_SIZE 50
#define PASSWORD_SIZE 50
#define NAME_SIZE 50
#define PHONE_SIZE 50

/* User data structure */
struct user
{
    char username[25];
    char phone_number[25];
    char account_number[ID_SIZE];
    char password[PASSWORD_SIZE];
    float balance;
};
/**
 * This function checks whether a file with a certain name exists in a directory. The file name is constructed by concatenating the string "./client/"
 * with the value of the temp parameter (a copy) which is appending the string ".dat" to the resulting string. The function then tries to open the file reading only,
 * and if it is successful, it closes the file and returns 1, indicating that the file exists. If the file cannot be opened, the function returns 0,
 * indicating that the file does not exist.
 */
int account_exist(char account_number[ID_SIZE])
{
    char temp[ID_SIZE];
    char current_filename[FILENAME_SIZE] = "./client/";
    strcpy(temp, account_number);
    strcat(current_filename, strcat(temp, ".dat"));
    FILE *file;

    if ((file = fopen(current_filename, "r")))
    {
        fclose(file);
        return 1;
    }
    return 0;
}

int password_verif(char account_number[ID_SIZE], char password[PASSWORD_SIZE])
{
    char temp[ID_SIZE];
    struct user current_user;
    char filename[FILENAME_SIZE] = "./client/";
    strcpy(temp, account_number);
    strcat(filename, strcat(temp, ".dat"));
    FILE *file;
    if ((file = fopen(filename, "r")))
    {
        fread(&current_user, sizeof(struct user), 1, file);
        fclose(file);

        if (!strcmp(password, current_user.password))
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

void display_information(struct user u)
{
    printf("\n\n-------------------------------");
    printf("\n\nInformation du compte %s", u.account_number);
    printf("\n\nTitulaire du compte: %s", u.username);
    printf("\nNumero de compte: %s", u.account_number);
    printf("\nNumero de telephone: %s", u.phone_number);
    printf("\nSolde: %f", u.balance);
    printf("\n-------------------------------\n\n");
}

struct user get_data(char account_number[ID_SIZE])
{
    char temp[ID_SIZE];
    struct user u;
    char filename[FILENAME_SIZE] = "./client/";
    strcpy(temp, account_number);
    strcat(filename, strcat(temp, ".dat"));
    FILE *file;
    if ((file = fopen(filename, "r")))
    {
        fread(&u, sizeof(struct user), 1, file);
        fclose(file);
        return u;
    }
    else
    {
        fprintf(stderr, "Indisponible...\n");
        exit(-1);
    }
}

struct user create_account()
{

    struct user new_user;
    char path[FILENAME_SIZE];
    char temp[ID_SIZE];
    FILE *fp;

    printf("Indiquer un identifiant:\t"); /* Pourra être generer automatiquement */
    scanf("%s", new_user.account_number);

    printf("Indiquer un nom:\t");
    scanf("%s", new_user.username);

    printf("Indiquer un numero de telephone:\t");
    scanf("%s", new_user.phone_number);

    printf("Indiquer un mot de passe:\t"); /* Pourra être generer automatiquement */
    scanf("%s", new_user.password);

    new_user.balance = 0;

    strcpy(path, "./client/");
    strcpy(temp, new_user.account_number);
    strcat(path, strcat(temp, ".dat"));
    fp = fopen(path, "w");
    fwrite(&new_user, sizeof(struct user), 1, fp);
    if (fwrite != 0)
    {
        printf("\nCompte numero %s ajoute!", &new_user.account_number);
        display_information(new_user);
        return new_user;
    }
    else
    {
        fprintf(stderr, "\nErreur dans la création du compte\n");
        exit(-1);
    }
    fclose(fp);
}

void get_date()
{
    time_t now = time(NULL);
    struct tm tm = *localtime(&now);
    printf("\nLe %02d-%02d-%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
}

int withdraw(struct user u)
{
    float amount;
    printf("\n\nMontant du retrait: \t");
    scanf("%f", &amount);
    u.balance += amount;

    /* Update data */
    char temp[ID_SIZE];
    char filename[FILENAME_SIZE] = "./client/";
    strcpy(temp, u.account_number);
    strcat(filename, strcat(temp, ".dat"));
    FILE *file;
    if ((file = fopen(filename, "w")))
    {
        fwrite(&u, sizeof(struct user), 1, file);
        if (fwrite != NULL)
        {
            fclose(file);
            return 1;
        }
    }
    else
    {
        return 0;
    }
}

char get_balance(char account_number[ID_SIZE])
{
    char temp[ID_SIZE];
    struct user u;
    char filename[FILENAME_SIZE] = "./client/";
    strcpy(temp, account_number);
    strcat(filename, strcat(temp, ".dat"));
    FILE *file;
    if ((file = fopen(filename, "r")))
    {
        fread(&u, sizeof(struct user), 1, file);
        fclose(file);
        return u.balance;
    }
    else
    {
        fprintf(stderr, "Solde indisponible...\n");
        exit(-1);
    }
}

int main()
{
    struct user usr;
    FILE *fp;
    char filename[FILENAME_SIZE];
    int opt;
    char id[ID_SIZE];
    char password[PASSWORD_SIZE];
    int connected = 0;

    system("cls");
    printf("Effectuer une operation: ");
    printf("\n\n1. Creer un compte");
    printf("\n2. Se connecter a mon espace");

    printf("\n\nSelectionnez une option: \t");
    scanf("%d", &opt);

    if (opt == 1)
    {

        /* Creation d'un compte */
        system("cls");
        usr = create_account();
    }
    else if (opt == 2)
    {
        system("cls");
        printf("\nVeuillez indiquer votre numero de compte: \t");
        scanf("%s", &id);
        /* Verification de l'identifiant */

        if (account_exist(id))
        {
            printf("\nMot de passe: \t");
            scanf("%s", &password);
            /* Verification du mot de passe  */
            if ((password_verif(id, password)))
            {
                connected = 1;
                system("cls");
                usr = get_data(id);
                printf("\nBienvenue dans votre espace %s", usr.username);
                while (connected == 1)
                {
                    system("cls");
                    printf("Effectuer une operation: ");
                    printf("\n\n1. Retrait");
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
                        if (withdraw(usr))
                        {
                            printf("\nRetrait effectue avec succes!");
                        }
                        break;

                    case 2:
                        printf("\n\nVotre solde: %d", get_balance(usr.account_number));
                        get_date();
                        break;
                    case 4:
                        display_information(usr);
                        get_date();
                        break;
                    }

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
