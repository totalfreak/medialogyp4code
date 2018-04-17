#include "Arduino.h"
#include "../Field/Field.h"
#include "Controller.h"




Field pentaFields[2][5] = {
  {Field(52, 22), Field(50, 24), Field(48, 26), Field(46, 28), Field(44, 30)},    // 0
  {Field(42, 22), Field(44, 24), Field(44, 26), Field(44, 28), Field(44, 30)}     // 1

};

unsigned long millisDiff = 0;
int timeDelta = 1000;

int amountOfBeats = 2;

int beatIterator = 0;

Controller controller = Controller(40);

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
  checkForKids();
  controller.read();
  if (controller.playing && (unsigned long)(millis() - millisDiff) > timeDelta) {
    millisDiff = (unsigned long)(millis());
    playBeats(beatIterator);
  }
}

void playBeats(int iterator) {
  Serial.println("Playing beat number: " + String(iterator));
  for(int i = 0; i < 5; i++) {
    pentaFields[iterator][i].play();
  }
  beatIterator++;
  if(beatIterator >= amountOfBeats) {
    beatIterator = 0;
    controller.playing = false;
  }

}

void checkForKids() {
  for(int i = 0; i < amountOfBeats; i++) {
    for(int j = 0; j < 5; j++) {
      pentaFields[i][j].read();
    }
  }
}
