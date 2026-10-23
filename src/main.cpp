#include "lcd.h"
#include "lcd_bus.h"

Lcd display;
GPIOLCDBus grio_bus;

void setup() {
  display.print_animated_line({.text = "Hello, c++ programming!",
                               .direction = LcdAnimationDirection::Left,
                               .infinite = true});
  display.print_animated_line({.text = "Hello lcd animations!",
                               .direction = LcdAnimationDirection::Rigth});
}

void loop() { display.tick(); }