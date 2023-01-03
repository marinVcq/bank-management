# Useful

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
