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

WireI2c arduino_default_i2c;

ArduinoGpioLcdBus::ArduinoGpioLcdBus(LcdGpioPins pins)
    : LcdGpioBus(arduino_bus_actions, arduino_lcd_timer, pins) {}

ArduinoLcd::ArduinoLcd(const ArduinoLcdConfig &config)
    : Lcd(LcdConfig{.bus = config.bus,
                    .timer = arduino_lcd_timer,
                    .rows_config = config.rows_config}) {}

ArduinoGpioLcd::ArduinoGpioLcd(const ArduinoGpioLcdConfig &config)
    : ArduinoGpioLcdBus(config.pins),
      ArduinoLcd({.bus = *this, .rows_config = config.rows_config}) {}

ArduinoPcf8574LcdBus::ArduinoPcf8574LcdBus(uint8_t address, I2c &i2c)
    : LcdPcdf8574Bus(address, i2c, arduino_bus_actions, arduino_lcd_timer) {};

ArduinoPcf8574Lcd::ArduinoPcf8574Lcd(const ArduinoPcf8574LcdConfig &config)
    : ArduinoPcf8574LcdBus(config.address, config.i2c),
      ArduinoLcd({.bus = *this, .rows_config = config.rows_config}) {}
