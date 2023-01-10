#include <windows.h>
#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 35
#define MIDDLE_WIDTH 40
#define MIDDLE_HEIGHT 25

// Global Handle
extern HANDLE hStdout;
extern HANDLE hStdin;
extern HANDLE hScreenMutex; // "Screen update" mutex
extern HANDLE menuThread;   // "Main thread update" mutex
extern int menu_option;

// Console variables
extern COORD console_size;
extern SMALL_RECT rect;
extern CONSOLE_FONT_INFOEX cfi;
extern CONSOLE_SCREEN_BUFFER_INFO default_console_info;
extern CONSOLE_CURSOR_INFO cursor_info;
extern WORD default_text_attributes;
extern INPUT_RECORD rc;
extern DWORD dwRead;
extern BOOL is_setup;

// Console input and output mode
extern DWORD dwOutMode;
extern DWORD dwInMode;

// Style functions
void print_spaces(int n);
void print_image(int x_pos, char *fn);
void print_color(char *string, char *color, int margin);
void print_date(void);
void line_break(int num);
void loading();
void fordelay(long long int j);

// Core functions
void setup_console(void *arg);
void menu(void *name);
void list_option(char opt_arr[][50], int selector, int size);
void restore_console(void);