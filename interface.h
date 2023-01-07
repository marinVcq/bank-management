#include <windows.h>
#define CONSOLE_WIDTH 80
#define CONSOLE_HEIGHT 50
#define MIDDLE_WIDTH 40
#define MIDDLE_HEIGHT 25

#define JUMP "\n"
#define DOUBLE_JUMP "\n\n"

extern HANDLE hStdout;
extern HANDLE hStdin;
extern HANDLE hScreenMutex; // "Screen update" mutex
extern HANDLE menuThread;   // "Main thread update" mutex
extern int menu_option;

extern CONSOLE_SCREEN_BUFFER_INFO default_console_info;
extern CONSOLE_CURSOR_INFO cursor_info;
extern WORD default_text_attributes;
extern INPUT_RECORD rc;
extern DWORD dwRead;

void setTimeout(int time_msec);
void setup_console(void);
void print_spaces(int n);
void print_date(void);
void fordelay(int j);
void print_image(int x_pos, char *fn);
void ResetConsoleColour(WORD Attributes);

void menu(void *name);

void list_option(char opt_arr[][50], int selector, int size);
void loading_screen();
void restore_console();
void print_color(char *string, char *color, int margin);
void line_break(int num);