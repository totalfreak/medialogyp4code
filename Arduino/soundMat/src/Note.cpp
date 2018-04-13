#include "Arduino.h"

class Note {
public:
  char name[3];
  float frequency;

  Note(char initName[2], float initFreq) {
    name[0] = initName[0];
    name[1] = initName[1];
    name[2] = '\0';

    frequency = initFreq;
  }

  Note(){};

  

private:

};
