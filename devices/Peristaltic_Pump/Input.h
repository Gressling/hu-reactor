#ifndef INPUT_H
#define INPUT_H

#include <Arduino.h>

class Input {
public:
  String getInput(String inputText);
  void message(String message);
};

#endif // INPUT_H
