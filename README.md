# Useful tips for WIN32 beginner

## color background and foreground

It's "color XX" where the first X is the background and the second X is the foreground.
system("color 70");

0 = Black   8 = Gray
1 = Blue    9 = Light Blue
2 = Green   A = Light Green
3 = Aqua    B = Light Aqua
4 = Red     C = Light Red
5 = Purple  D = Light Purple
6 = Yellow  E = Light Yellow
7 = White   F = Bright White

## center the program in terminal

COORD coord;
coord.X = width / 2;
coord.Y = height / 2;
SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

### The console screen buffer info structure

<https://learn.microsoft.com/en-us/windows/console/console-screen-buffer-info-str>

typedef struct _CONSOLE_SCREEN_BUFFER_INFO {
  COORD      dwSize;
  COORD      dwCursorPosition;
  WORD       wAttributes;
  SMALL_RECT srWindow;
  COORD      dwMaximumWindowSize;
} CONSOLE_SCREEN_BUFFER_INFO;

- dwSize A COORD structure that contains the size of the console screen buffer, in character columns and rows.

- dwCursorPosition. A COORD structure that contains the column and row coordinates of the cursor in the console screen buffer.

wAttributes
The attributes of the characters written to a screen buffer by the WriteFile and WriteConsole functions, or echoed to a screen buffer by the ReadFile and ReadConsole functions. For more information, see Character Attributes.

srWindow
A SMALL_RECT structure that contains the console screen buffer coordinates of the upper-left and lower-right corners of the display window.

dwMaximumWindowSize
A COORD structure that contains the maximum size of the console window, in character columns and rows, given the current screen buffer size and font and the screen size.

### The console font structure

typedef struct _CONSOLE_FONT_INFOEX {
  ULONG cbSize;
  DWORD nFont;
  COORD dwFontSize;
  UINT  FontFamily;
  UINT  FontWeight;
  WCHAR FaceName[LF_FACESIZE];
} CONSOLE_FONT_INFOEX, *PCONSOLE_FONT_INFOEX;

cbSize
The size of this structure, in bytes. This member must be set to sizeof(CONSOLE_FONT_INFOEX) before calling GetCurrentConsoleFontEx or it will fail.

nFont
The index of the font in the system's console font table.

dwFontSize
A COORD structure that contains the width and height of each character in the font, in logical units. The X member contains the width, while the Y member contains the height.

FontFamily
The font pitch and family. For information about the possible values for this member, see the description of the tmPitchAndFamily member of the TEXTMETRIC structure.

FontWeight
The font weight. The weight can range from 100 to 1000, in multiples of 100. For example, the normal weight is 400, while 700 is bold.

FaceName
The name of the typeface (such as Courier or Arial).

BOOL WINAPI SetCurrentConsoleFontEx(
  _In_ HANDLE               hConsoleOutput,
  _In_ BOOL                 bMaximumWindow,
  _In_ PCONSOLE_FONT_INFOEX lpConsoleCurrentFontEx
);

code example:

``int main(int argc, char** args)
{
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof cfi;
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;
    cfi.dwFontSize.Y = 20;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    printf("A quick brown fox jumps over the lazy dog\n");

    printf("Setting to Lucida Console: press <Enter> ");
    getchar();
    wcscpy(cfi.FaceName, L"Lucida Console");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    printf("Setting to Consolas: press <Enter> ");
    getchar();
    wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);

    printf("Press <Enter> to exit");
    getchar();
    return 0;
}``

## main menu selector

based on C++

  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  // you can loop k higher to see more color choices
  for(int k = 1; k < 255; k++)
  {
    // pick the colorattribute k you want
    SetConsoleTextAttribute(hConsole, k);
    cout << k << " I want to be nice today!" << endl;
  }

  

  press arrow get the keycode and nav

  #include <windows.h>
   #include <stdio.h>

void SetConsoleColour(WORD*Attributes, DWORD Colour)
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
    WORD Attributes = 0;
    SetConsoleColour(&Attributes, FOREGROUND_INTENSITY | FOREGROUND_RED);
    printf("Foreground change..\n");
    ResetConsoleColour(Attributes);

    printf("Normal attributes..\n");

    SetConsoleColour(&Attributes, BACKGROUND_INTENSITY | BACKGROUND_RED);
    printf("Background change..\n");
    ResetConsoleColour(Attributes);
    printf("Normal attributes..\n");


    SetConsoleColour(&Attributes, FOREGROUND_INTENSITY | FOREGROUND_RED);
    printf("Mixture");
    ResetConsoleColour(Attributes);
    printf(" of ");
    SetConsoleColour(&Attributes, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
    printf("both..\n");
    ResetConsoleColour(Attributes);


    SetConsoleColour(&Attributes, FOREGROUND_INTENSITY | FOREGROUND_RED);
    printf("Mixture");
    ResetConsoleColour(Attributes);
    printf(" of ");
    SetConsoleColour(&Attributes, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN);
    printf("all ");
    ResetConsoleColour(Attributes);
    SetConsoleColour(&Attributes, BACKGROUND_INTENSITY | BACKGROUND_BLUE);
    printf("three");
    ResetConsoleColour(Attributes);
    printf(" ");
    SetConsoleColour(&Attributes, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE | FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    printf("in a single line\n");
    ResetConsoleColour(Attributes);
}

## read input from buffer

<https://learn.microsoft.com/en-us/windows/console/reading-input-buffer-events>

typedef struct _KEY_EVENT_RECORD {
  BOOL  bKeyDown;
  WORD  wRepeatCount;
  WORD  wVirtualKeyCode;
  WORD  wVirtualScanCode;
  union {
    WCHAR UnicodeChar;
    CHAR  AsciiChar;
  } uChar;
  DWORD dwControlKeyState;
} KEY_EVENT_RECORD;

l'idée est de recuperer l'attribut wVirtualKeyCode
ici on s'interesse aux touches arrow donc aux keycodes:

- VK_LEFT 0x25 LEFT ARROW key
- VK_UP 0x26 UP ARROW key
- VK_RIGHT 0x27 RIGHT ARROW key
- VK_DOWN 0x28 DOWN ARROW key

<https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes>
