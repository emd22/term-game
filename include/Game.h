#pragma once
#include <vector>
#include <string>

#include "ColorText.h"
#include "TerminalUtil.h"
#include "Unicode.h"

struct Pixel {
    u32char style;
    int color;
};

class Gamespace
{
public:
    Gamespace(int width, int height)
    {
        m_width = width;
        m_height = height;
        
        u32char background_light = char8to32("░");
        
        for (int y = 0; y < height; y++) {
            std::vector<Pixel> row;
            for (int x = 0; x < width; x++) {
                Pixel pix;
                pix.color = DEFAULT_COLOR; // grey
                pix.style = background_light;
                row.push_back(pix);
            }
            gs.push_back(row);
        }
    }

    void edit(int x, int y, int change, int color)
    {
        gs[y][x].style = change;
        gs[y][x].color = color;
    }

    void print(int x = 0, int y = 0)
    {
        SetTermCursorPos(x, y);
        for (int i = 0; i < gs.size(); i++) {
            auto &row = gs[i];
            for (int j = 0; j < row.size(); j++) {
                // covert ints to chars
                // todo: make it use utf-8 on linux
                ColoredText(row[j].style, row[j].color);
            }
            SetTermCursorPos(x, y + i);
        }

    }
    void reset(u32char c)
    {
        for (auto &row : gs) {
            for (Pixel &pix : row) {
                pix.color = DEFAULT_COLOR; // grey
                pix.style = c;
            }
        }


    }

    int GetWidth() { return m_width; }
    int GetHeight() { return m_height; }

    
private:
    std::vector<std::vector<Pixel>> gs;
    int m_width;
    int m_height;
};