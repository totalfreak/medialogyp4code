#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "../ButtonLib/Button.h"

class Controller {
public:
  bool playing = false, counting = false;
  int playButtonPin, metroPin, BPM, octaveUpPin, octaveDownPin, octaveIterator = 4;

  Button playButton = Button(1, true, true, 50);
  Button octaveUpButton = Button(1, true, true, 50);
  Button octaveDownButton = Button(1, true, true, 50);

  Controller(int InitPlayButtonPin, int initMetroPin, int initBPM, int initOctaveUpPin, int initOctaveDownPin);

  Controller();

  void read();

  void octaveUp();
  void octaveDown();

  void countIn();

};

#endif
