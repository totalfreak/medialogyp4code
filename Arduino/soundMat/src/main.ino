#include "Arduino.h"
#include "Activator.cpp"
#include "ButtonLib/Button.h"

Button activators[] = {Button(52, true, true, 100)};

//How many activators there are
const int amountOfActivators = 1;

//Temp buzzerpin
int buzzerPin = 10;

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
      Serial.println("Shit");
    }
  }
}
