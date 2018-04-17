#ifndef Field_h
#define Field_h

#include "../Note/Note.h"
#include "../ButtonLib/Button.h"

class Field {
public:
  int pin, belaPin, soundDuration = 300;
  bool hasKid = false;
  Button button = Button(1, true, true, 50);
  //Note arrays (Scales)
  Field(int initPin, int initBelaPin);
  Field();

  void read();
  void play();
  void stopPlay();

};
#endif
