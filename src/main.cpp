#include <Arduino.h> // need to be imported for links to setup and loop functionsl

#include "arduino_platform.h"
#include "character_animation.h"
#include "hd44780.h"
#include "i2c_wire.h"
#include "lcd.h"
#include "lcd_pcf8574_bus.h"

ArduinoClock clock;
WireI2c wire;
LcdPcf8574Bus bus(0x27, wire, clock);
Hd44780 driver(bus, clock);
Lcd lcd(driver);

void setup() {
  lcd.init();
  lcd.print_animated_line(
      {
          .text = "Hello, c++ programming!",
          .direction = CharacterAnimationDirection::Left,
      },
      millis());
  lcd.print_animated_line({.text = "Hello lcd animations!",
                           .direction = CharacterAnimationDirection::Rigth},
                          millis());
}

void loop() { lcd.tick(millis()); }
