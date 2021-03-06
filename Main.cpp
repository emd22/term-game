#include <iostream>
#include <ctime>
#include <cstdlib>
#include <chrono>
#include <thread>

#include <locale.h>
#include <memory>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <ctime>

#include "include/GetCh.h"
#include "include/ColorText.h"
#include "include/Game.h"
#include "include/Entity.h"
#include "include/Unicode.h"
#include "include/Util.h"
#include "include/xml_writer.h"
#include "include/beep.h"
#include "include/random_music.h"

typedef std::shared_ptr<Entity> EntityPtr;

struct BeeperFixer {
  BeeperFixer()
  {
    open_beeper();
  }

  ~BeeperFixer()
  {
    can_beep = false;
    beep(200, 400);
    close_beeper();
    ucout << "Booper\n";
  }
};

int main()
{
    srand(std::time(0));

    open_beeper();
    atexit(close_beeper);

    std::thread beep_thread(beep_func);

    int update_counter = 0;
    bool play_music = true;

    #ifdef _WIN32
    int temp = system("cls");
    #else
    int temp1 = system("sudo modprobe pcspkr");
    int temp2 = system("clear");
    int temp3 = system("stty -echo");
    #endif

    utf8_init();
    bool pause_collisions = false;
    srand(time(0));

    HideTermCaret();

    int x = 1, y = 2;
    Gamespace gamespace(45, 15);

    std::vector<EntityPtr> entities;

    for (int i = 0; i < 5; i++) {
        entities.push_back(EntityPtr(new NPC(gamespace,
            randrange(0, gamespace.GetWidth() - 1),  //Random x
            randrange(0, gamespace.GetHeight() - 1), //Random y
            char8to32("ӭ"),                          //Style
            45,                                      //Color
            true,                                    //Collision
            1)));                                    //Size
    }

    for (int x = 0; x < gamespace.GetWidth() + 1; x++) {
        entities.push_back(EntityPtr(new Entity(gamespace, x, 1, char8to32("▄"), 15, true)));
    }
    for (int x = 0; x < gamespace.GetWidth() + 1; x++) {
        entities.push_back(EntityPtr(new Entity(gamespace, x, gamespace.GetHeight() - 1, char8to32("▀"), 15, true)));
    }
    for (int y = 0; y < gamespace.GetHeight() + 1; y++) {
        entities.push_back(EntityPtr(new Entity(gamespace, 0, y, char8to32("▐"), 15, true)));
    }
    for (int y = 0; y < gamespace.GetHeight() + 1; y++) {
        entities.push_back(EntityPtr(new Entity(gamespace, gamespace.GetWidth() - 1, y, char8to32("▌"), 15, true)));
    }

    XmlWriter writer("res/out.xml");
    writer.beginDocument();
    writer.beginElement("gamespace");
    for (EntityPtr &entity_ptr : entities) {
      if (entity_ptr != nullptr) {
        // save entity
        writer.beginElement("entity");
          writer.attribute("x", std::to_string(entity_ptr->GetX()));
          writer.attribute("y", std::to_string(entity_ptr->GetY()));
          writer.attribute("style", std::to_string(entity_ptr->GetStyle()));
        writer.endElement();
      }
    }
    writer.endElement();
    writer.close();

    while (true) {
        update_counter++;

        int old_x = x;
        int old_y = y;
        SetTermCursorPos(0, 0);

        char getchar = my_getch();
        ucout << "\n";

        if (getchar == 'w') {
            y--;
        }
        if (getchar == 'a') {
            x--;
        }
        if (getchar == 's') {
            y++;
        }
        if (getchar == 'd') {
            x++;
        }
        if (getchar == 'q') {
            beep_thread.detach();
            beep(500, 1500);
            int temp4 = system("stty echo");
            return 0;
        }

        if (getchar) {
            pause_collisions = false;
        }
        gamespace.reset(char8to32("░"));

        for (auto &entity : entities) {

            entity->UpdateAI(gamespace);

            // draw the entity
            for (int sx = 0; sx < entity->GetSize(); sx++) {
                for (int sy = 0; sy < entity->GetSize(); sy++) {

                    gamespace.edit(clamp(0, gamespace.GetWidth() - 1, entity->GetX() + sx),
                        clamp(0, gamespace.GetHeight() - 1, entity->GetY() + sy),
                                   entity->GetStyle(),
                                   entity->GetColor());

                    if (entity->GetIsCollidable() == true && pause_collisions == false) {
                        if (x == entity->GetX() + sx && y == entity->GetY() + sy) {
                            x = old_x;
                            y = old_y;
                        }
                    }
                }
            }


        }
        gamespace.edit(x, y, char8to32("☻"), DEFAULT_COLOR);

        gamespace.print(4, 4);

        std::chrono::duration<float> time_to_sleep(0.05f);
        std::this_thread::sleep_for(time_to_sleep);
        if (pause_collisions == false) {
            pause_collisions = true;
        }

        if (update_counter == 25){
          update_counter = 0;
        }


    }

    return 0;
}
