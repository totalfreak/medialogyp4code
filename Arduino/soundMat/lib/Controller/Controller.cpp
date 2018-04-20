#include "Arduino.h"
#include "Controller.h"

#define MAXOCTAVE 6
#define MINOCTAVE 3

Controller::Controller(int initPlayButtonPin, int initMetroPin, int initBPM, int initOctaveUpPin, int initOctaveDownPin) {
  playButtonPin = initPlayButtonPin;
  metroPin = initMetroPin;
  BPM = initBPM;
  octaveUpPin = initOctaveUpPin;
  octaveDownPin = initOctaveDownPin;
  pinMode(metroPin, OUTPUT);
  playButton = Button(playButtonPin, true, true, 50);
  octaveUpButton = Button(octaveUpPin, true, true, 50);
  octaveDownButton = Button(octaveDownPin, true, true, 50);
}

Controller::Controller() {};

void Controller::read() {
  playButton.read();
  octaveUpButton.read();
  octaveDownButton.read();

  if(octaveUpButton.wasPressed() && octaveIterator < MAXOCTAVE) {
    octaveUp();
    Serial.println("UP: " + String(octaveIterator));
  }

  if(octaveDownButton.wasPressed() && octaveIterator > MINOCTAVE) {
    octaveDown();
    Serial.println("DOWN: " + String(octaveIterator));
  }

  if(playButton.wasPressed() && playing && !counting) {
    playing = false;
  } else
  if (playButton.wasPressed() && !playing && !counting) {
    countIn();
  }
}

void Controller::octaveUp() {
  octaveIterator++;
  digitalWrite(13, HIGH);
  delay(10);
  digitalWrite(13, LOW);
}

void Controller::octaveDown() {
  octaveIterator--;
  digitalWrite(12, HIGH);
  delay(10);
  digitalWrite(12, LOW);
}

void Controller::countIn() {
  counting = true;
  for(int i = 0; i < 4; i++) {
    digitalWrite(metroPin, HIGH);
    delay(BPM/2);
    digitalWrite(metroPin, LOW);
    delay(BPM/2);
  }
  playing = true;
  counting = false;
}
