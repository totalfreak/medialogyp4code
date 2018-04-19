#include "Arduino.h"
#include "Field.h"

Field::Field(int initPin, int initBelaPin, int initBPM) {
  pin = initPin;
  belaPin = initBelaPin;
  button = Button(pin, true, true, 50);
  BPM = initBPM;
  pinMode(belaPin, OUTPUT);
}


void Field::read() {
  button.read();
  if (button.wasPressed()) {
    hasKid = true;
    if(!isPlaying && !isCountingIn) {
      play();
      delay(BPM-50);
      stopPlay();
    }
  } else if(button.wasReleased()) {
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
