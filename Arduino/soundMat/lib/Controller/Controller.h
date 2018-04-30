#ifndef Controller_h
#define Controller_h

#include "Arduino.h"
#include "../ButtonLib/Button.h"

class Controller {
public:
  bool playing = false, counting = false, recording = false;
  int playButtonPin, metroPin, BPM, octaveUpPin, octaveDownPin, octaveIterator = 4, recButtonPin, resetButtonPin;
  int segmentLedPins[4] = {A7, A8, A9, A10};

  Button playButton = Button(1, true, true, 50);
  Button octaveUpButton = Button(1, true, true, 50);
  Button octaveDownButton = Button(1, true, true, 50);
  Button recButton = Button(1, true, true, 50);
  Button resetButton = Button(1, true, true, 50);


  Controller(int InitPlayButtonPin, int initMetroPin, int initBPM, int initOctaveUpPin, int initOctaveDownPin, int initRecButtonPin, int initResetButtonPin);

  Controller();

  void read();

  int returnSegmentSelectorState();

  void octaveUp();
  void octaveDown();

  void countIn(bool shouldRecord);

};

#endif
