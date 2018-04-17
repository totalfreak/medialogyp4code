#include "Arduino.h"
#include "../Field/Field.h"
#include "Controller.h"



// Field(buttonPin, belaPin);
// belaPins
//  22 = C
//  24 = D
//  26 = E
//  28 = F
//  30 = h
Field pentaFields[2][5] = {
  {Field(52, 22), Field(50, 24), Field(48, 26), Field(46, 28), Field(44, 30)},
  {Field(42, 22), Field(40, 24), Field(38, 26), Field(36, 28), Field(34, 30)}

};

unsigned long millisDiff = 0;
  //BPM = 120
int BPM = 120;
int soundDuration;

int amountOfBeats = 2;

int beatIterator = 0;

Controller controller = Controller(53);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    BPM = (1.0 / BPM)*60*1000;
    soundDuration = BPM-50;
}

void loop() {
  checkForKids();
  controller.read();
  if (controller.playing) {
    //Start playing beat
    if((unsigned long)(millis() - millisDiff) > BPM)  {
      millisDiff = (unsigned long)(millis());
      playBeat(beatIterator);
    }
  }
}

void playBeat(int iterator) {
  Serial.println("Playing beat number: " + String(iterator));
  for(int i = 0; i < 5; i++) {
    pentaFields[iterator][i].play();
  }
  delay(soundDuration);
  stopBeat(beatIterator);
  beatIterator++;
  if(beatIterator >= amountOfBeats) {
    beatIterator = 0;
    controller.playing = false;
  }
}

void stopBeat(int iterator) {
  for(int i = 0; i < 5; i++) {
    pentaFields[iterator][i].stopPlay();
  }
}


void checkForKids() {
  for(int i = 0; i < amountOfBeats; i++) {
    for(int j = 0; j < 5; j++) {
      pentaFields[i][j].read();
    }
  }
}
