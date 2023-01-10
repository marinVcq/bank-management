#define PATH_SIZE 50
#define ACCOUNT_NO_SIZE 50
#define PASSWORD_SIZE 50
#define NAME_SIZE 50
#define PHONE_SIZE 50

#define MAX_CUSTOMER_THREADS 2

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

// Handle for customer operation
extern HANDLE h_ctm_threads[MAX_CUSTOMER_THREADS];
extern int customer_threads;

void create_account(void *arg);
void login_account(void *arg);

int check_account(char customer_account_no[ACCOUNT_NO_SIZE]);
int check_password(char customer_account_no[ACCOUNT_NO_SIZE], char password[PASSWORD_SIZE]);

// Account operations
void handle_operation(void *selector);
void customer_info_get(void *customer_account_no);
struct customer record_get(char customer_account_no[ACCOUNT_NO_SIZE]);
void withdraw_process(void *customer_account_no);
void balance_get(void *customer_account_no);
void transfer_process(void *customer_account_no);

BOOL update_information(struct customer customer);
