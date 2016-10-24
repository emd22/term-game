#pragma once
#include "Unicode.h"
#include "Util.h"
#include "Game.h"
#include <string.h>

class Entity
{
public:
    Entity(Gamespace &gs, int x, int y, u32char style, int color, bool is_collidable = true, int size = 1)
        
    {
        m_x = x;
        m_y = y;
        m_dest_x = randrange(0, gs.GetWidth());
        m_dest_y = randrange(0, gs.GetHeight());
        m_style = style;
        m_color = color;
        m_size = size;
        m_is_collidable = is_collidable;
    }

    Entity(const Entity &other)
    {
        m_x = other.m_x;
        m_y = other.m_y;
        m_dest_x = other.m_dest_x;
        m_dest_y = other.m_dest_y;
        m_style = other.m_style;
        m_color = other.m_color;
        m_size = other.m_size;
        m_is_collidable = other.m_is_collidable;
    }

    bool CheckCollision(int other_x, int other_y)
    {

    }

    int GetX()
    {
        return m_x;
    }
    int GetY()
    {
        return m_y;
    }
    u32char GetStyle()
    {
        return m_style;
    }
    int GetColor()
    {
        return m_color;
    }
    int GetSize()
    {
        return m_size;
    }
    bool GetIsCollidable()
    {
        return m_is_collidable;
    }
    
    
    virtual void UpdateAI(Gamespace &gs)
    {
    }
protected:
    int m_x;
    int m_y;
    
    int m_dest_x;
    int m_dest_y;
    
    u32char m_style;
    int m_color;
    int m_size;
    bool m_is_collidable;

};



class NPC : public Entity
{
public:
    NPC(Gamespace &gs, int x, int y, u32char style, int color, bool is_collidable = true, int size = 1)
        : Entity(gs, x, y, style, color, is_collidable, size)
    {
        
        m_dest_x = randrange(0, gs.GetWidth());
        m_dest_y = randrange(0, gs.GetHeight());
    }
    
    NPC(const NPC &other) 
        : Entity(other)
    {
        m_dest_x = other.m_dest_x;
        m_dest_y = other.m_dest_y;
    }
    int speed = 30;
    int wait = 0;
    int max_wait = 50-speed;
    
    virtual void UpdateAI(Gamespace &gs)
    {
        
        /*
         * if currentPosition == destination
         *     destination = a random position
         * 
         * each frame, move the entity closer to the destination
         */
        wait++;
        if (wait == max_wait){
            int diff_x = m_dest_x - m_x;
            int diff_y = m_dest_y - m_y;
            
            diff_x = clamp(-1, 1, diff_x);
            diff_y = clamp(-1, 1, diff_y);
            
            m_x += diff_x;
            m_y += diff_y;
            
            if (m_x == m_dest_x && m_y == m_dest_y) {
                m_dest_x = randrange(0, gs.GetWidth() - 2);
                m_dest_y = randrange(0, gs.GetHeight() - 2);
            }
            wait = 0;
        }
        
    }
    
};