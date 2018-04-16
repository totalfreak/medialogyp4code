#include "Arduino.h"
#include "Field.h"

Field::Field(int initPin, Note initNote, int initBuzzerPin) {
  note = initNote;
  pin = initPin;
  buzzerPin = initBuzzerPin;
  button = Button(pin, true, true, 50);
  tone.begin(buzzerPin);
}


void Field::read() {
  button.read();
  if (button.isPressed()) {
    hasKid = true;
  } else {
    hasKid = false;
  }
}


void Field::play() {
  if(hasKid) {
    tone.play(note.frequency, soundDuration);
  }
}

float Field::getFrequency() {
  float freq = 0;
  if(hasKid) {
    freq = note.frequency;
  }
  return freq;
}
