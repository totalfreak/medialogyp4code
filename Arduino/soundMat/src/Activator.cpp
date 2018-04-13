#include "Arduino.h"
#include "NewTone/NewTone.h"

//Button object
class Activator {
public:

  //Button(ButtonPin, LEDPin)
  //Constructor
  //Also creating own LED object
  Activator(int initPin, int initMelody) {
    pin = initPin;
    melody = initMelody;
    setPinMode(INPUT_PULLUP);
  }

  Activator(){};

  //Returning digital value of button press
  int readValue() {
    if(digitalRead(getPin())) {
      return HIGH;
    } else
      return LOW;
  }

  //Playing my sound
  void playSound(int buzzerPin, int duration) {
    NewTone(buzzerPin, melody, duration);
  }

  //Setting pin mode
  void setPinMode(int mode) {
    pinMode(getPin(), mode);
  }

  //Activating button, and LED
  void activate() {
    setState(true);
  }

  void deActivate() {
    setState(false);
  }

  bool getState() {
    return state;
  }

  void setState(bool newState) {
    state = newState;
  }

  bool getLastState() {
    return lastState;
  }

  void setLastState(bool newState) {
    lastState = newState;
  }


  int getPin() {
    return pin;
  }

private:
  int pin, melody;
  bool state = false, lastState = true;
};
