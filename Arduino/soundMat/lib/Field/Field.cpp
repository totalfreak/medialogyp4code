#include "Arduino.h"
#include "Field.h"

Field::Field(int initPin, int initBelaPin, int initBPM) {
  pin = initPin;
  belaPin = initBelaPin;
  button = Button(pin, true, true, 50);
  BPM = initBPM;
  soundDuration = BPM-50;
  pinMode(belaPin, OUTPUT);
}


void Field::read() {
  button.read();
  if (button.wasPressed()) {
    hasKid = true;
  } else if(button.wasReleased()) {
    hasKid = false;
    hasPlayedPreview = false;
  }
}


void Field::play() {
  if(hasKid) {
    digitalWrite(belaPin, HIGH);
  }
}

void Field::stopPlay() {
  digitalWrite(belaPin, LOW);
}
