#include "Note.h"

Note::Note(char initName[2], float initFreq) {
  name[0] = initName[0];
  name[1] = initName[1];
  name[2] = '\0';

  frequency = initFreq;
}

Note::Note(){};
