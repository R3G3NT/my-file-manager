#pragma once
#include <Windows.h>

extern HANDLE hConsole;

enum Colors
{
    BLACK = 0,
    GREEN = 2,
    LIGHT_GREY = 7,
    GREY = 8,
    LIGHT_GREEN = 10,
    WHITE = 15
};

void SetCursorBlinking(bool isBlinking);