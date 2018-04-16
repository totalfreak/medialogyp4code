#ifndef Field_h
#define Field_h

#include "../Note/Note.h"
#include "../ButtonLib/Button.h"
#include "../RogueTone/Tone.h"

class Field {
public:
  int pin, buzzerPin, soundDuration = 300;
  bool hasKid = false;
  Note note;
  Tone tone;
  Button button = Button(1, true, true, 50);
  //Note arrays (Scales)
  Field(int initPin, Note initNote, int initBuzzerPin);
  Field();

  void read();
  void play();

  float getFrequency();

};
#endif
