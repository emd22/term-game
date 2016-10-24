#pragma once

#ifdef _WIN32

#include <windows.h>

inline void SetTermCursorPos(int x, int y)
{
    COORD cur;
    cur.X = x;
    cur.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
}

inline void HideTermCaret()
{
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO structCursorInfo;
    // Set the size of the cursor, and make the cursor invisible.
    // This is strange, because the cursor will only be hidden
    // if the size of the cursor is changed in combination with
    // setting bVisible to FALSE.
    // If you do not set the cursor size, the cursor will remain
    // visible, even though you've set bVisible = FALSE!
    structCursorInfo.dwSize = 1;
    structCursorInfo.bVisible = FALSE;

    // Set the cursor info settings for the Standard Output Handle.
    SetConsoleCursorInfo(hConsoleOutput,
        &structCursorInfo);
}

#else

#include <cstdio>

//ansi escape barf

inline void SetTermCursorPos(int x, int y)
{
    std::printf("\033[%d;%dH", y, x);
}


inline void HideTermCaret()
{
    std::fputs("\e[?25l", stdout);
}
#endif