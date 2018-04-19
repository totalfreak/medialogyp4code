#ifndef Field_h
#define Field_h

#include "../ButtonLib/Button.h"

class Field {
public:
  int pin, belaPin, soundDuration = 0, BPM;
  bool hasKid = false, isPlaying = false, isCountingIn = false, hasPlayedPreview = false;
  Button button = Button(1, true, true, 50);
  //Note arrays (Scales)
  Field(int initPin, int initBelaPin, int initBPM);
  Field();

  void read();
  void play();
  void stopPlay();

};
#endif
