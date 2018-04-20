#include "Arduino.h"
#include <EEPROM.h>
#include "../Field/Field.h"
#include "Controller.h"
         //BPM = 120
int BPM = (1.0 / 120 )*60*1000;

struct SequenceField {
  int octave = 4;
  Field activatedFields[5];
};

struct Segment {
  bool enabled = true;
  int ledPin;
  SequenceField sequence[4];
};

// Field(buttonPin, belaPin);
// belaPins
//  22 = C
//  24 = D
//  26 = E
//  28 = G
//  30 = A
Field pentaFields[4][5] = {
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)},
  {Field(52, 22, BPM), Field(50, 24, BPM), Field(48, 26, BPM), Field(46, 28, BPM), Field(44, 30, BPM)}
};

unsigned long millisDiff = 0;

int soundDuration;

int amountOfBeats = 4;

int beatIterator = 0;

//Amount of possible segements
int amountOfSegments = 4;

Segment segments[4];

Controller controller = Controller(53, 31, BPM, A0, A1, A2);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    soundDuration = BPM-50;
    Serial.println(sizeof(Segment));
    //Reset segments;
    //writeSegments();
    readSegments();
}

void loop() {
  checkForKids();
  playPreviews();
  stopPreviews();
  controller.read();
  if (controller.playing || controller.recording) {
    //Start playing beat
    if((unsigned long)(millis() - millisDiff) > BPM)  {
      millisDiff = (unsigned long)(millis());
      playBeat(beatIterator, 0);
    }
  }
}

void playBeat(int iterator, int segmentSelector) {
  Serial.println("Playing beat number: " + String(iterator));
  if(!controller.recording) {
    Serial.println("Before: " + String(controller.octaveIterator));
    int octaveDiff = segments[segmentSelector].sequence[iterator].octave - controller.octaveIterator;
    octaveChangeroo(octaveDiff);
    Serial.println("After: " + String(controller.octaveIterator));
  }

  for(int i = 0; i < 5; i++) {
    if (controller.recording) {
      pentaFields[iterator][i].read();
      controller.read();
      segments[segmentSelector].sequence[iterator].activatedFields[i] = Field(0,0,0);
      if(pentaFields[iterator][i].hasKid) {
        pentaFields[iterator][i].play();
        segments[segmentSelector].sequence[iterator].activatedFields[i] = pentaFields[iterator][i];
        segments[segmentSelector].sequence[iterator].octave = controller.octaveIterator;
        Serial.println(segments[segmentSelector].sequence[iterator].activatedFields[i].pin);
      }
    } else {
      if (!segments[segmentSelector].enabled) {
        continue;
      }
      segments[segmentSelector].sequence[iterator].activatedFields[i].play();
    }
  }
  delay(soundDuration);
  stopBeat(beatIterator, segmentSelector);
  beatIterator++;
  if(beatIterator >= amountOfBeats) {
    if(controller.recording)
      writeSegments();
    beatIterator = 0;
    controller.playing = false;
    controller.recording = false;
  }
}

void stopBeat(int iterator, int segmentSelector) {
  for(int i = 0; i < 5; i++) {
    pentaFields[iterator][i].stopPlay();
    segments[segmentSelector].sequence[iterator].activatedFields[i].stopPlay();
  }
}

void writeSegments() {
  for(int i = 0; i < amountOfSegments; i++) {
    EEPROM.put(i*sizeof(Segment), segments[i]);
  }
}

//Read and save highscores in highscores array
void readSegments() {
  for(int i = 0; i < amountOfSegments; i++) {
    EEPROM.get(i*sizeof(Segment), segments[i]);
  }
}

void octaveChangeroo(int diff) {
  if(diff > 0) {
    for(int i = 0; i < diff; i++) {
      controller.octaveUp();
    }
  } else if(diff < 0){
    for(int i = 0; i > diff; i--) {
      controller.octaveDown();
    }
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
