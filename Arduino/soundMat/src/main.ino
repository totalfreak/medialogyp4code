#include "Arduino.h"
#include "ButtonLib/Button.h"
#include "Note.cpp"
#include "NewTone/NewTone.h"

Button activators[] = {Button(52, true, true, 50)};

//How many activators there are
const int amountOfActivators = 1;

//Temp buzzerpin
int buzzerPin = 36;
//Note arrays (Scales)
Note pentaTones[] = {Note("C4", 261.63), Note("D4", 293.67), Note("E4", 329.63), Note("G4", 392.00), Note("A4", 440), Note("C5", 523.25), Note("D5", 587.33)};
Note hepTones[5][7] = {
{Note("C1", 32.70), Note("D1", 36.70), Note("E1", 41.20), Note("F1", 43.65) ,Note("G1", 48.99), Note("A1", 55.00), Note("h1", 61.73)},
{Note("C2", 65.41), Note("D2", 73.41), Note("E2", 82.40), Note("F2", 87.31) ,Note("G2", 97.99), Note("A2", 110), Note("h2", 123.47)},
{Note("C3", 130.81), Note("D3", 146.83), Note("E3", 164.80), Note("F3", 174.61) ,Note("G3", 195.99), Note("A3", 220), Note("h3", 246.94)},
{Note("C4", 261.63), Note("D4", 293.67), Note("E4", 329.63), Note("F4", 349.23) ,Note("G4", 392.00), Note("A4", 440), Note("h4", 493.88)},
{Note("C5", 523.25), Note("D5", 587.33), Note("E5", 659.25), Note("F5", 698.46) ,Note("G5", 783.99), Note("A5", 880), Note("h5", 987.77)}};

int noteIterator = 0, octaveIterator = 2;

//How long a tone should play
int timeDelta = 300;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
// put your main code here, to run repeatedly:
  for(int i = 0; i < amountOfActivators; i++) {
    activators[i].read();
    if (activators[i].wasPressed()) {
      Serial.println(hepTones[octaveIterator][noteIterator].frequency);
      NewTone(buzzerPin, hepTones[octaveIterator][noteIterator].frequency, 100);
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
