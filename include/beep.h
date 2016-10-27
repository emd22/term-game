#pragma once

#include <vector>
#include <map>
#include <array>

#include "note.h"
#include "random_music.h"

bool can_beep = true;

#ifdef _WIN32
#include <WinBase.h>
void beep(int freq = 440, int len = 200){
    Beep(freq, len);
}
#else
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/kd.h>
#include <sys/ioctl.h>
#include <thread>
#include <iostream>

int beeper;

void open_beeper()
{
    beeper = open("/dev/console", O_WRONLY);
}

void close_beeper()
{
  ioctl(beeper, KIOCSOUND, 0);
  close(beeper);
}

void beep(int freq = 440, int len = 200){
    if (freq > 0 && freq < 2000){
      ioctl(beeper, KIOCSOUND, (int)(1193180/freq));
      std::this_thread::sleep_for(std::chrono::milliseconds(len));
      ioctl(beeper, KIOCSOUND, 0);
    }

}

#endif

class NotePlayer {
public:
  inline int GetFrequency(Note note) const { return frequencies.at(note); }

  void PlayNote(Note note, int len) const
  {
    beep(frequencies.at(note), len);
  }

  const std::map<Note, int> frequencies = {
    { NOTE_C, 523 },
    { NOTE_CSHARP, 554},
    { NOTE_D, 587},
    { NOTE_DSHARP, 622},
    { NOTE_E, 659},
    { NOTE_F, 698},
    { NOTE_FSHARP, 740},
    { NOTE_G, 784},
    { NOTE_GSHARP, 831},
    { NOTE_A, 880},
    { NOTE_ASHARP, 932},
    { NOTE_B, 998},
    { NOTE_HIGHC, 1046 },
    { NOTE_HIGHCSHARP, 1108 },
    { NOTE_HIGHD, 1175 },
    { NOTE_HIGHDSHARP, 1244 },
    { NOTE_HIGHE, 1318 },
  };

  const std::array<Note, 7> c_major_scale = std::array<Note, 7> {
    NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_G, NOTE_A, NOTE_B
  };
};

void beep_func() {
  NotePlayer note_player;

  bool do_backwards = false;
  bool backwards = false;
  int i = 0;

  /*std::vector<Note> song = {
    NOTE_A, NOTE_A, NOTE_HIGHC, NOTE_A,
    NOTE_A, NOTE_A, NOTE_B, NOTE_A,
    NOTE_A, NOTE_A, NOTE_HIGHC, NOTE_A,
    NOTE_A, NOTE_A, NOTE_HIGHE, NOTE_HIGHD,
  };
  std::vector<Note> song = {
    NOTE_E, NOTE_A, NOTE_G, NOTE_A,
    NOTE_G, NOTE_E, NOTE_A, NOTE_G,
    NOTE_A, NOTE_G, NOTE_HIGHC, NOTE_A,
    NOTE_A, NOTE_A, NOTE_HIGHE, NOTE_HIGHD,
  };

  // cool creepy song
  std::vector<Note> song = {
    NOTE_A, NOTE_B, NOTE_CSHARP, NOTE_A, NOTE_B, NOTE_CSHARP,
    NOTE_A, NOTE_B, NOTE_CSHARP, NOTE_A, NOTE_B, NOTE_CSHARP,
    NOTE_B, NOTE_C, NOTE_HIGHD, NOTE_B, NOTE_C, NOTE_HIGHD,
    NOTE_B, NOTE_C, NOTE_HIGHD, NOTE_B, NOTE_C, NOTE_HIGHD,
  };*/

  std::vector<Note> song;
  generate_random_song(song, 15);

  std::cout << "NOTES: \n";
  for (auto &note : song) {
    std::cout << "\t" << note << "\n";
  }

  while (can_beep) {

    if (do_backwards) {
      if (!backwards && i == note_player.c_major_scale.size() - 1) {
        backwards = true;
      } else if (backwards && i == 0) {
        backwards = false;
      }
    } else {
      if (i == song.size()) {
        i = 0;
      }
    }

    Note note = song.at(/*randrange(0, 6)*/ i);
    //std::cout << "got note: " << note << "\t\t\n";
    note_player.PlayNote(note, randrange(40, 200));

    if (do_backwards && backwards) {
      i--;
    } else {
      i++;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}
