#pragma once

#include <stdint.h>

using PinNumber = uint8_t;

class Gpio {
public:
  virtual ~Gpio() = default;

  virtual void write(PinNumber pin, bool high) = 0;
  virtual void set_output(PinNumber pin) = 0;
  virtual void set_input(PinNumber pin) = 0;
};
