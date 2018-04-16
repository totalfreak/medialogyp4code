#ifndef Field_h
#define Field_h

#include "../Note/Note.h"
#include "../ButtonLib/Button.h"
#include "../NewTone/NewTone.h"

class Field {
public:
  int pin, buzzerPin, soundDuration;
  Note note;

  float timeDelta;
  Button button = Button(1, true, true, 50);
  //Note arrays (Scales)
  Field(int initPin, Note initNote);

  Field();

void read();


void play();

};
#endif
