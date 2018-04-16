#include "Arduino.h"
#include "Controller.h"

Controller::Controller(int initPlayButtonPin) {
  playButtonPin = initPlayButtonPin;

  playButton = Button(playButtonPin, true, true, 50);
}

void Controller::read() {
  playButton.read();
  if (playButton.wasPressed()) {
    playing = true;
  }
}
