#include "Arduino.h"
#include "Field.h"

Field::Field(int initPin, int initBelaPin) {
  pin = initPin;
  belaPin = initBelaPin;
  button = Button(pin, true, true, 50);
  pinMode(belaPin, OUTPUT);
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
    digitalWrite(belaPin, HIGH);
  }
}

void Field::stopPlay() {
  digitalWrite(belaPin, LOW);
}
