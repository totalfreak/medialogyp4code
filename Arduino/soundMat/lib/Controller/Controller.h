#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "../ButtonLib/Button.h"

class Controller {
public:
  bool playing = false;
  int playButtonPin;

  Button playButton = Button(1, true, true, 50);

  Controller(int InitPlayButtonPin);

  Controller();

  void read();

};

#endif
