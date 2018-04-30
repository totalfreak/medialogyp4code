#include "Arduino.h"
#include "Controller.h"

#define MAXOCTAVE 6
#define MINOCTAVE 3

Controller::Controller(int initPlayButtonPin, int initMetroPin, int initBPM, int initOctaveUpPin, int initOctaveDownPin, int initRecButtonPin, int initResetButtonPin) {
  metroPin = initMetroPin;
  BPM = initBPM;

  pinMode(metroPin, OUTPUT);

  for(int i = 0; i < 4; i++) {
    pinMode(segmentLedPins[i], OUTPUT);
  }


  playButton = Button(initPlayButtonPin, true, true, 50);
  octaveUpButton = Button(initOctaveUpPin, true, true, 50);
  octaveDownButton = Button(initOctaveDownPin, true, true, 50);
  recButton = Button(initRecButtonPin, true, true, 50);
  resetButton = Button(initResetButtonPin, true, true, 59);
}

Controller::Controller() {};

void Controller::read() {
  playButton.read();
  octaveUpButton.read();
  octaveDownButton.read();
  recButton.read();
  resetButton.read();
  if(octaveUpButton.wasPressed() && octaveIterator < MAXOCTAVE) {
    octaveUp();
    Serial.println("UP: " + String(octaveIterator));
  }

  if(octaveDownButton.wasPressed() && octaveIterator > MINOCTAVE) {
    octaveDown();
    Serial.println("DOWN: " + String(octaveIterator));
  }


  if(recButton.wasPressed() && !playing && !counting && !recording) {
    countIn(true);
  }
  if (playButton.wasPressed() && !playing && !counting && !recording) {
    countIn(false);
  }
}

void Controller::octaveUp() {
  octaveIterator++;
  digitalWrite(12, HIGH);
  delay(5);
  digitalWrite(12, LOW);
  delay(5);
}

void Controller::octaveDown() {
  octaveIterator--;
  digitalWrite(11, HIGH);
  delay(5);
  digitalWrite(11, LOW);
  delay(5);
}

void Controller::countIn(bool shouldRecord) {
  counting = true;
  for(int i = 0; i < 4; i++) {
    digitalWrite(metroPin, HIGH);
    delay(BPM/2);
    digitalWrite(metroPin, LOW);
    delay(BPM/2);
  }
  recording = shouldRecord;
  playing = !shouldRecord;
  counting = false;
}
