#include "Arduino.h"
#include "Controller.h"

Controller::Controller(int initPlayButtonPin, int initBelaPin, int initBPM) {
  playButtonPin = initPlayButtonPin;
  belaPin = initBelaPin;
  BPM = initBPM;
  pinMode(belaPin, OUTPUT);
  playButton = Button(playButtonPin, true, true, 50);
}

Controller::Controller() {};

void Controller::read() {
  playButton.read();
  if (playButton.wasPressed() && !playing && !counting) {
    countIn();
  }
}

void Controller::countIn() {
  counting = true;
  for(int i = 0; i < 4; i++) {
    digitalWrite(belaPin, HIGH);
    delay(BPM/2);
    digitalWrite(belaPin, LOW);
    delay(BPM/2);
  }
  playing = true;
  counting = false;
}
