#include "Arduino.h"
#include "../Field/Field.h"
#include "Controller.h"
         //BPM = 120
int BPM = (1.0 / 120 )*60*1000;

// Field(buttonPin, belaPin);
// belaPins
//  22 = C
//  24 = D
//  26 = E
//  28 = G
//  30 = A
Field pentaFields[8][5] = {
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)}
};

unsigned long millisDiff = 0;

int soundDuration;

int amountOfBeats = 8;

int beatIterator = 0;

Controller controller = Controller(53, 31, BPM);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    soundDuration = BPM-50;
}

void loop() {
  checkForKids();
  playPreviews();
  stopPreviews();
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

void playPreviews() {
  for(int i = 0; i < amountOfBeats; i++) {
    for(int j = 0; j < 5; j++) {
      if(pentaFields[i][j].hasKid && pentaFields[i][j].soundDuration == 0 && !controller.playing && !pentaFields[i][j].hasPlayedPreview) {
        pentaFields[i][j].hasPlayedPreview = true;
        pentaFields[i][j].play();
        pentaFields[i][j].soundDuration = BPM-50;

      }
      if(pentaFields[i][j].soundDuration > 0) {
        pentaFields[i][j].soundDuration -= 1;
      }
    }
  }
}

void stopPreviews() {
    for(int i = 0; i < amountOfBeats; i++) {
      for(int j = 0; j < 5; j++) {
        if(!controller.playing && pentaFields[i][j].soundDuration <= 0) {
          pentaFields[i][j].stopPlay();
      }
    }
  }
}


void checkForKids() {
  for(int i = 0; i < amountOfBeats; i++) {
    for(int j = 0; j < 5; j++) {
      pentaFields[i][j].read();
      pentaFields[i][j].isPlaying = controller.playing;
      pentaFields[i][j].isCountingIn = controller.counting;
    }
  }
}
