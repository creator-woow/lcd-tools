#include <Arduino.h>

#include "lcd_arduino.h"

LcdBusActions arduino_bus_actions{
    .digital_write = [](uint8_t pin, bool high) { digitalWrite(pin, high); },
    .set_output_pin = [](uint8_t pin) { pinMode(pin, OUTPUT); },
    .set_input_pin = [](uint8_t pin) { pinMode(pin, INPUT); },
};

LcdTimer arduino_lcd_timer{
    .milliseconds_now = millis,
    .delay_microseconds = delayMicroseconds,
    .delay_milliseconds = delay,
};

ArduinoGpioLcdBus::ArduinoGpioLcdBus(LcdGpioPins pins)
    : LcdGpioBus(arduino_bus_actions, arduino_lcd_timer, pins) {}

ArduinoLcd::ArduinoLcd(const ArduinoLcdConfig &config)
    : Lcd(LcdConfig{.bus = config.bus,
                    .timer = arduino_lcd_timer,
                    .rows_config = config.rows_config}) {}

ArduinoGpioLcdBus ArduinoGpioLcd::init_bus(LcdGpioPins pins) {
  return ArduinoGpioLcdBus{pins};
}

ArduinoGpioLcd::ArduinoGpioLcd(const ArduinoGpioLcdConfig &config)
    : ArduinoLcd(
          {.bus = init_bus(config.pins), .rows_config = config.rows_config}) {}
