#include "Arduino.h"
#include "Note.cpp"
#include "ButtonLib/Button.h"
#include "NewTone/NewTone.h"




class Field {
public:
  float frequency;
  Button activators[] = {Button(52, true, true, 50)};
  //Note arrays (Scales)
  Note pentaTones[] = {Note("C4", 261.63), Note("D4", 293.67), Note("E4", 329.63), Note("G4", 392.00), Note("A4", 440), Note("C5", 523.25), Note("D5", 587.33)};
  Note hepTones[5][7] = {
  {Note("C1", 32.70), Note("D1", 36.70), Note("E1", 41.20), Note("F1", 43.65) ,Note("G1", 48.99), Note("A1", 55.00), Note("h1", 61.73)},
  {Note("C2", 65.41), Note("D2", 73.41), Note("E2", 82.40), Note("F2", 87.31) ,Note("G2", 97.99), Note("A2", 110), Note("h2", 123.47)},
  {Note("C3", 130.81), Note("D3", 146.83), Note("E3", 164.80), Note("F3", 174.61) ,Note("G3", 195.99), Note("A3", 220), Note("h3", 246.94)},
  {Note("C4", 261.63), Note("D4", 293.67), Note("E4", 329.63), Note("F4", 349.23) ,Note("G4", 392.00), Note("A4", 440), Note("h4", 493.88)},
  {Note("C5", 523.25), Note("D5", 587.33), Note("E5", 659.25), Note("F5", 698.46) ,Note("G5", 783.99), Note("A5", 880), Note("h5", 987.77)}};

  Field() {


  }

  Field(){};



private:

};
