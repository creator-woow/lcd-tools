#include <Arduino.h>

#include "lcd.h"

Lcd display;

void setup() {
  display = Lcd(LcdConfig{
    .pins = {
      .pin_e = 11,  
      .pin_rs = 12,
      .pin_d4 = 2,
      .pin_d5 = 3,
      .pin_d6 = 4,
      .pin_d7 = 5
    },
    .rows_config = {
      .amount = 2,
      .width = 16,
    }
  });
  display.print_animated_line({ .text = "Hello, c++ programming!", .direction = LcdAnimationDirection::Left, .infinite = true });
  display.print_animated_line({ .text = "Hello lcd animations!", .direction = LcdAnimationDirection::Rigth  });
}

void loop() {
  display.tick();
}