#include <Arduino.h>

#include "arduino_platform.h"

Millisecond ArduinoClock::milliseconds_now() { return millis(); }

void ArduinoClock::delay_milliseconds(Millisecond milliseconds) {
  delay(milliseconds);
}

void ArduinoClock::delay_microseconds(Microsecond microseconds) {
  delayMicroseconds(microseconds);
}

void ArduinoGpio::write(PinNumber pin, bool high) { digitalWrite(pin, high); }

void ArduinoGpio::set_output(PinNumber pin) { pinMode(pin, OUTPUT); }

void ArduinoGpio::set_input(PinNumber pin) { pinMode(pin, INPUT); }
