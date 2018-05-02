#include "Arduino.h"
#define FASTLED_INTERRUPT_RETRY_COUNT 1
#include "../lib/fastled/FastLED.h"
#include <EEPROM.h>
#include "../Field/Field.h"
#include "Controller.h"

#ifdef __AVR__
  #include <avr/power.h>
#endif

         //BPM = 120
int BPM = (1.0 / 140 )*60*1000;

#define NUM_LEDS 300
#define DATA_PIN 9
#define CLOCK_PIN 8
#define LED_COLOUR_ORDER BGR
#define LED_STRIP_SIZE 19
#define LED_STRIP_OFFSET 215


CRGB leds[NUM_LEDS];

CRGB bgColor = CRGB::Black;
CRGB lightUpColor = CRGB::Orange;

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
  {Field(44, 22, BPM), Field(46, 24, BPM), Field(48, 26, BPM), Field(50, 28, BPM), Field(52, 30, BPM)},
  {Field(42, 22, BPM), Field(40, 24, BPM), Field(38, 26, BPM), Field(36, 28, BPM), Field(34, 30, BPM)},
  {Field(6, 22, BPM), Field(5, 24, BPM), Field(4, 26, BPM), Field(3, 28, BPM), Field(2, 30, BPM)},
  {Field(A15, 22, BPM), Field(A14, 24, BPM), Field(A13, 26, BPM), Field(A12, 28, BPM), Field(A11, 30, BPM)}
};

//Segment selector buttons
Button segementSelectors[4] = {Button(A3, true, true, 50), Button(A4, true, true, 50), Button(A5, true, true, 50), Button(A6, true, true, 50)};


// Used in keeping track of time when changing beats
unsigned long millisDiff = 0;
float deltaTime = 0.0;

//How long the each beat should play
int soundDuration;

//Amount of beats
int amountOfBeats = 4;

//Int used for iterating over the beats
int beatIterator = 0;
int segmentIterator = 0;
int stripIterator = 1000;

//Amount of possible segements
int amountOfSegments = 4;

Segment segments[4];

//The controller object
//Controller(playButton pin, metronome pin, BPM, octaveUp pin, octaveDown pin, recordButton pin)
Controller controller = Controller(53, 31, BPM, A0, A1, A2, 51);

void setup() {
    Serial.begin(9600);
    soundDuration = BPM-50;
    Serial.println(sizeof(Segment));

    //Reset segments
    //writeSegments();

    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, LED_COLOUR_ORDER, DATA_RATE_MHZ(12)>(leds, NUM_LEDS);
    FastLED.setBrightness(20);
    //FastLED.setDither(1);

    for(int i = 0; i < LED_STRIP_OFFSET; i++) {
      leds[i] = bgColor;
    }
    for(int i = 0; i < amountOfBeats; i++) {
      int min = i * LED_STRIP_SIZE + LED_STRIP_OFFSET;
      int max = (i+1) * LED_STRIP_SIZE + LED_STRIP_OFFSET;

      for(int j = min; j < min+3; j++) {
        leds[j] = CRGB::Red;
      }
      for(int j = max; j < max+3; j++) {
        leds[j] = CRGB::Red;
      }
    }

    FastLED.show();
    delay(60);

    //Reading the segments into segments[]
    readSegments();
    selectorStateChanger();

}

void loop() {


  checkForKids();
  stopPreviews();
  playPreviews();

  controller.read();

  if(!controller.playing && !controller.recording && controller.resetButton.wasPressed()) {
    resetSegments();
  }

  if((unsigned long)(millis() - millisDiff) > BPM)  {
    deltaTime = (unsigned long)(millis() - millisDiff);
    //Serial.println(deltaTime);
    millisDiff = (unsigned long)(millis());
    if (controller.playing || controller.recording) {

      //Start playing beat of each enabled segment
      if(segments[segmentIterator].enabled) {
          for(int i = 0; i < amountOfSegments; i++) {
            digitalWrite(controller.segmentLedPins[i], LOW);
          }
          digitalWrite(controller.segmentLedPins[segmentIterator], HIGH);

          playBeat(beatIterator, segmentIterator);
          millisDiff = (unsigned long)(millis());
        } else {
          segmentIterator++;
          if(segmentIterator >= amountOfSegments) {
            segmentIterator = 0;
            beatIterator = 0;
            controller.playing = false;
            controller.recording = false;
          }
        }
      }
    }

  if(!controller.playing && !controller.recording && !controller.counting) {
    selectorStateChanger();
  }
}

void selectorStateChanger() {
  for(int i = 0; i < amountOfSegments; i++) {
    segementSelectors[i].read();
    if(segementSelectors[i].wasPressed()) {
      segments[i].enabled = !segments[i].enabled;
      writeSegments();
      Serial.println("Changed state of segment " + String(i) + " to " + String(segments[i].enabled));
    }
    digitalWrite(controller.segmentLedPins[i], segments[i].enabled);
  }
}

void playBeat(int iterator, int segmentSelector) {
  Serial.println("Playing beat number: " + String(iterator));
  if(!controller.recording) {
    //Getting the difference between the current octave and the desired octave, and changes it accordingly.
    int octaveDiff = segments[segmentSelector].sequence[iterator].octave - controller.octaveIterator;
    octaveChangeroo(octaveDiff);
  }

  for(int i = 0; i < 5; i++) {
    if (controller.recording) {
      pentaFields[iterator][i].read();
      controller.read();
      segments[segmentSelector].sequence[iterator].activatedFields[i] = Field(0,0,0);
      if(pentaFields[iterator][i].hasKid) {
        segments[segmentSelector].sequence[iterator].activatedFields[i] = pentaFields[iterator][i];
        segments[segmentSelector].sequence[iterator].octave = controller.octaveIterator;
        segments[segmentSelector].sequence[iterator].activatedFields[i].play();
      }
    } else {
      if (!segments[segmentSelector].enabled) {
        continue;
      }
      segments[segmentSelector].sequence[iterator].activatedFields[i].play();
    }
  }

  showStrip(iterator);
  delay(soundDuration);
  hideStrip(iterator);
  stopBeat(beatIterator, segmentSelector);

  beatIterator++;
  if(beatIterator >= amountOfBeats) {
    beatIterator = 0;
    segmentIterator++;

    if(segmentIterator >= amountOfSegments) {
      if(controller.recording)
        writeSegments();
      segmentIterator = 0;
      controller.playing = false;
      controller.recording = false;
    }
  }
}

void stopBeat(int iterator, int segmentSelector) {
  for(int i = 0; i < 5; i++) {
    //pentaFields[iterator][i].stopPlay();
    segments[segmentSelector].sequence[iterator].activatedFields[i].stopPlay();
  }
}

void resetSegments() {
  for(int i = 0; i < amountOfSegments; i++) {
    if(!segments[i].enabled)
      continue;
    segments[i] = Segment();
    Serial.println("Reset segment: " + String(i) + " to " + String(segments[i].enabled));
  }
  writeSegments();
}

//Iterating through EEPROM and writing the segments
void writeSegments() {
  for(int i = 0; i < amountOfSegments; i++) {
    EEPROM.put(i*sizeof(Segment), segments[i]);
  }
}

//Read and save the segments from the EEPROM to the segments array
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

void showStrip(int iterator) {
  //FastLED.clear(true);
  int min = iterator * LED_STRIP_SIZE + LED_STRIP_OFFSET;
  int max = (iterator+1) * LED_STRIP_SIZE + LED_STRIP_OFFSET;

  for(int i = min+3; i < max; i++) {
    leds[i] = lightUpColor;
  }

  FastLED.show();
}

void hideStrip(int iterator) {
  int min = iterator * LED_STRIP_SIZE + LED_STRIP_OFFSET;
  int max = (iterator+1) * LED_STRIP_SIZE + LED_STRIP_OFFSET;

  for(int i = min+3; i < max; i++) {
    leds[i] = bgColor;
  }
  FastLED.show();
}

void playPreviews() {
  for(int i = 0; i < amountOfBeats; i++) {
    for(int j = 0; j < 5; j++) {
      if(pentaFields[i][j].hasKid && !controller.playing && !controller.counting && !controller.recording && pentaFields[i][j].soundDuration <= 0 && !pentaFields[i][j].hasPlayedPreview) {
        for(int g = 0; g < amountOfBeats; g++) {
          pentaFields[g][j].soundDuration = BPM-50;
        }

        pentaFields[i][j].hasPlayedPreview = true;
        pentaFields[i][j].play();
      }
      if(pentaFields[i][j].soundDuration > 0) {
         pentaFields[i][j].soundDuration -= 50;
         Serial.println(String(pentaFields[i][j].soundDuration) + " " + "Field " + String(i) + "," + String(j));
      }
    }
  }
}

void stopPreviews() {
    for(int i = 0; i < amountOfBeats; i++) {
      for(int j = 0; j < 5; j++) {
        if(!controller.playing && !controller.recording && !controller.counting && pentaFields[i][j].soundDuration <= 0) {
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
