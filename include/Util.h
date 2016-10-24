#pragma once

inline int randrange(int min, int max)
{
    return min + (rand() % (int)(max - min + 1));
}

inline int clamp(int min, int max, int val)
{
    if (val > max) {
        return max;
    } else if (val < min) {
        return min;
    }
    return val;
}