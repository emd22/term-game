#pragma once

#include <map>
#include <array>

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
    ioctl(beeper, KIOCSOUND, (int)(1193180/freq));
    std::this_thread::sleep_for(std::chrono::milliseconds(len));
    ioctl(beeper, KIOCSOUND, 0);
}

#endif


enum Note {
    NOTE_C,
    NOTE_CSHARP,
    NOTE_D,
    NOTE_DSHARP,
    NOTE_E,
    NOTE_F,
    NOTE_FSHARP,
    NOTE_G,
    NOTE_GSHARP,
    NOTE_A,
    NOTE_ASHARP,
    NOTE_B,
    NOTE_HIGHC,
    NOTE_HIGHCSHARP,
    NOTE_HIGHD,
    NOTE_HIGHDSHARP,
    NOTE_HIGHE,
};

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
