#include "Console.h"
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

void SetCursorBlinking(bool isBlinking)
{
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(hConsole, &structCursorInfo);
    structCursorInfo.bVisible = isBlinking;
    SetConsoleCursorInfo(hConsole, &structCursorInfo);
}