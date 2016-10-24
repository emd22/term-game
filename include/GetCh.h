#pragma once

#ifdef _WIN32

#include <conio.h>

char my_getch()
{
    if (_kbhit()) {
        return (char)_getch();
    }
    return '\0';
}

#else


#include <unistd.h>
#include <termios.h>

#include <stdio.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <stropts.h>

int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;
    
    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }
    
    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}


char my_getch()
{
    if (_kbhit()) {
        struct termios oldattr, newattr;
        int ch;
        tcgetattr( STDIN_FILENO, &oldattr );
        newattr = oldattr;
        newattr.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
        ch = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
        return ch;
    } else {
        return '\0';
        
    }
}

#endif