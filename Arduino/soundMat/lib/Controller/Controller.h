#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "../ButtonLib/Button.h"

class Controller {
public:
  bool playing = false, counting = false;
  int playButtonPin, belaPin, BPM;

  Button playButton = Button(1, true, true, 50);

  Controller(int InitPlayButtonPin, int initBelaPin, int initBPM);

  Controller();

  void read();

  void countIn();

};

#endif
