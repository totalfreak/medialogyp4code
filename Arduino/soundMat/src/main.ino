#include "Arduino.h"
#include "../Field/Field.h"
#include "../Note/Note.h"
#include "Controller.h"


Note pentaTones[5][5] = {
  {Note("C3", 130.81), Note("D3", 146.83), Note("E3", 164.80), Note("G3", 195.99), Note("A3", 220)},    // 0
  {Note("C4", 261.63), Note("D4", 293.67), Note("E4", 329.63), Note("G4", 392.00), Note("A4", 440)},    // 1
  {Note("C5", 523.25), Note("D5", 587.33), Note("E5", 659.25), Note("G5", 783.99), Note("A5", 880)},    // 2
  {Note("C6", 1046.5), Note("D6", 1174.7), Note("E6", 1318.5), Note("G6", 1568.0), Note("A6", 1760.0)}, // 3
  {Note("C7", 2093.0), Note("D7", 2349.3), Note("E7", 2637.0), Note("G7", 3136.0), Note("A7", 3520.0)}  // 4
};

Field pentaFields[2][5] = {
  {Field(52, Note("C3", 130.81), 36), Field(50, Note("D3", 146.83), 34), Field(48, Note("E3", 164.80), 36), Field(46, Note("G3", 195.99), 34), Field(44, Note("A3", 220), 36)},    // 0
  {Field(42, Note("C3", 130.81), 36), Field(44, Note("D3", 146.83), 34), Field(44, Note("E3", 164.80), 36), Field(44, Note("G3", 195.99), 34), Field(44, Note("A3", 220), 36)}     // 1

};

Note hepTones[7][5] = {
  {Note("C3", 130.81), Note("D3", 146.83), Note("E3", 164.80), Note("F3", 174.61) ,Note("G3", 195.99)}, // 0
  {Note("A3", 220), Note("h3", 246.94), Note("C4", 261.63), Note("D4", 293.67), Note("E4", 329.63)},    // 1
  {Note("F4", 349.23) ,Note("G4", 392.00), Note("A4", 440), Note("h4", 493.88), Note("C5", 523.25)},    // 2
  {Note("D5", 587.33), Note("E5", 659.25), Note("F5", 698.46) ,Note("G5", 783.99), Note("A5", 880)},    // 3
  {Note("h5", 987.77), Note("C6", 1046.5), Note("D6", 1174.7), Note("E6", 1318.5), Note("F6", 1396.9)}, // 4
  {Note("G6", 1568.0), Note("A6", 1760.0), Note("h6", 1975.5), Note("C7", 2093.0), Note("D7", 2349.3)}, // 5
  {Note("E7", 2637.0), Note("F7", 2793.0), Note("G7", 3136.0), Note("A7", 3520.0), Note("h7", 3951.1)}  // 6
};

Note chromaticTones[12][5] = {
  {Note("C3", 130.81), Note("CIS3", 138.59), Note("D3", 146.83), Note("DIS3", 155.56), Note("E3", 164.80)}, // 0
  {Note("F3", 174.61), Note("FIS3", 185.00), Note("G3", 195.99), Note("GIS3", 207.65), Note("A3", 220)},    // 1
  {Note("AIS3", 233.08), Note("h3", 246.94), Note("C4", 261.63), Note("CIS4", 277.18), Note("D4", 293.67)}, // 2
  {Note("DIS4", 311.13), Note("E4", 329.63), Note("F4", 349.23), Note("FIS4", 369.99), Note("G4", 392.00)}, // 3
  {Note("GIS4", 415.30), Note("A4", 440), Note("AIS4", 466.16), Note("h4", 493.88), Note("C5", 523.25)},    // 4
  {Note("CIS5", 554.37), Note("D5", 587.33), Note("DIS5", 622.25), Note("E5", 659.25), Note("F5", 698.46)}, // 5
  {Note("FIS5", 739.99), Note("G5", 783.99), Note("GIS5", 830.61), Note("A5", 880), Note("AIS5", 932.33)},  // 6
  {Note("h5", 987.77), Note("C6", 1046.5), Note("CIS6", 1108.7), Note("D6", 1174.7), Note("DIS6", 1244.5)}, // 7
  {Note("E6", 1318.5), Note("F6", 1396.9), Note("FIS6", 1480.0), Note("G6", 1568.0), Note("GIS6", 1661.2)}, // 8
  {Note("A6", 1760.0), Note("AIS6", 1864.7), Note("h6", 1975.5), Note("C7", 2093.0), Note("CIS7", 2217.5)}, // 9
  {Note("D7", 2349.3), Note("DIS7", 2489.0), Note("E7", 2637.0), Note("F7", 2793.0), Note("FIS7", 2960.0)}, // 10
  {Note("G7", 3136.0), Note("GIS7", 3322.4), Note("A7", 3520.0), Note("AIS7", 3729.3), Note("h7", 3951.1)}  // 11
};

unsigned long millisDiff = 0;
int timeDelta = 1000;

int amountOfBeats = 2;

int beatIterator = 0;

float frequenciesToPlay[5];

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
