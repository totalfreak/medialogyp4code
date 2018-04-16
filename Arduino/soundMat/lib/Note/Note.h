#ifndef Note_h
#define Note_h

#include "Arduino.h"
class Note {
public:
  char name[3];
  float frequency;

  Note(char initName[2], float initFreq);

  Note();

};
#endif
