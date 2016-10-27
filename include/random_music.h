#pragma once

#include "Util.h"
#include "note.h"
#include <vector>

#define AMOUNT_NOTES 17

/*std::vector<Note> song = {
  NOTE_A, NOTE_B, NOTE_CSHARP, NOTE_A, NOTE_B, NOTE_CSHARP,
  NOTE_A, NOTE_B, NOTE_CSHARP, NOTE_A, NOTE_B, NOTE_CSHARP,
  NOTE_B, NOTE_C, NOTE_HIGHD, NOTE_B, NOTE_C, NOTE_HIGHD,
  NOTE_B, NOTE_C, NOTE_HIGHD, NOTE_B, NOTE_C, NOTE_HIGHD,
};*/

void generate_random_song(std::vector<Note> &song, int size) {
    song.clear();
    for (int i = 0; i < size; i++) {
        int starting_num = randrange(0, AMOUNT_NOTES - 1);
        int neg = randrange(0, 1);
        int max_size = 0;

        if (neg) {
            max_size = randrange(0, starting_num);
        }
        else {
            max_size = randrange(0, AMOUNT_NOTES-starting_num);
        }

        for (int j = 0; j < max_size; j++){

            song.push_back((Note)starting_num);
            if (neg) {
                starting_num--;
            }
            else {
                starting_num++;
            }
        }
    }
}
