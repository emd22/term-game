#pragma once
#include "Unicode.h"
#include <iostream>
#include <string>

#ifdef _WIN32

#define DEFAULT_COLOR 15

#include <windows.h>   // WinApi header

void ColoredText(u32char ch, int color)
{
    HANDLE hConsole;
    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hConsole, color);
    // print the unicode character
    // get it as a utf-8 character
    char as_utf8[4] = {'\0'};
    char32to8(ch, as_utf8);
    Utf8String utf8_string(as_utf8);
    ucout << utf8_string;

    SetConsoleTextAttribute(hConsole, DEFAULT_COLOR);
}

#else

#define DEFAULT_COLOR 0

void ColoredText(u32char ch, int color)
{
    char as_utf8[4] = {'\0'};
    char32to8(ch, as_utf8);
    Utf8String utf8_string(as_utf8);

    ucout << "\033[" << color << "m" << utf8_string << "\033[15m";
    //ucout << utf8_string;
}

#endif
