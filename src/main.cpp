#include <Arduino.h> // need to be imported for links to setup and loop functionsl

#include "lcd_arduino.h"

ArduinoGpioLcd display;

void setup() {
  display.begin();
  display.print_animated_line({.text = "Hello, c++ programming!",
                               .direction = LcdAnimationDirection::Left,
                               .infinite = true});
  display.print_animated_line({.text = "Hello lcd animations!",
                               .direction = LcdAnimationDirection::Rigth});
}

void loop() { display.tick(); }
