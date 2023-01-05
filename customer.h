#define PATH_SIZE 50
#define ACCOUNT_NO_SIZE 50
#define PASSWORD_SIZE 50
#define NAME_SIZE 50
#define PHONE_SIZE 50

/* Customer data structure */
struct customer
{
    char name[50];
    char phone_number[50];
    char account_number[ACCOUNT_NO_SIZE];
    char password[PASSWORD_SIZE];
    float balance;
};

/* User login informations*/
extern char account_number[ACCOUNT_NO_SIZE];
extern char password[PASSWORD_SIZE];
extern int connected;

int check_account(char customer_account_no[ACCOUNT_NO_SIZE]);
int check_password(char customer_account_no[ACCOUNT_NO_SIZE], char password[PASSWORD_SIZE]);
void customer_info(char customer_account_no[ACCOUNT_NO_SIZE]);
struct customer get_record(char customer_account_no[ACCOUNT_NO_SIZE]);
void create_account(void);
int withdraw(char customer_account_no[ACCOUNT_NO_SIZE]);
int get_balance(char customer_account_no[ACCOUNT_NO_SIZE]);
void login_account(void);
