#include "Arduino.h"
#include "Field.cpp"

Field fields[] = {Field()};

//How many activators there are
const int amountOfFields = 1;

//Temp buzzerpin
int buzzerPin = 36;


int noteIterator = 0, octaveIterator = 2;

//How long a tone should play
int timeDelta = 300;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {



// put your main code here, to run repeatedly:
  for(int i = 0; i < amountOfFields; i++) {
    activators[i].read();
    if (activators[i].wasPressed()) {
      Serial.println(hepTones[octaveIterator][noteIterator].frequency);
      NewTone(buzzerPin, hepTones[octaveIterator][noteIterator].frequency, timeDelta);
      noteIterator++;
      if(noteIterator > 6) {
        noteIterator = 0;
        octaveIterator++;
        if(octaveIterator > 4)
          octaveIterator = 2;
        }
    }
  }
}
