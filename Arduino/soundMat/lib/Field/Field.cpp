#include "Arduino.h"
#include "Field.h"

Field::Field(int initPin, Note initNote) {
  pin = initPin;
  note = initNote;
  buzzerPin = 36;
  soundDuration = 100;
  button = Button(pin, true, true, 50);
}

void Field::read() {
  button.read();
}


void Field::play() {
  NewTone(buzzerPin, note.frequency, soundDuration);
}
