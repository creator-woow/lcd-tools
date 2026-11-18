#pragma once

#include "clock.h"
#include "gpio.h"

class ArduinoClock : public Clock {
public:
  Millisecond milliseconds_now() override;
  void delay_milliseconds(Millisecond milliseconds) override;
  void delay_microseconds(Microsecond microseconds) override;
};

class ArduinoGpio : public Gpio {
public:
  void write(PinNumber pin, bool high) override;
  void set_output(PinNumber pin) override;
  void set_input(PinNumber pin) override;
};