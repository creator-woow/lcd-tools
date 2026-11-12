#pragma once

#include <stdint.h>

#include "lcd_timer.h"

using DigitalWriteHandler = void (*)(uint8_t pin, bool high);
using SetOutputPinModeHandler = void (*)(uint8_t pin);
using SetInputPinModeHandler = void (*)(uint8_t pin);
using DelayMicroseconds = void (*)(unsigned int microseconds);

struct LcdBusActions {
  DigitalWriteHandler digital_write;
  SetOutputPinModeHandler set_output_pin;
  SetInputPinModeHandler set_input_pin;
};

class LcdBus {
public:
  explicit LcdBus(const LcdBusActions &actions, const LcdTimer &timer);
  virtual ~LcdBus() = default;

  virtual void send_high_nibble(uint8_t byte, bool is_data) const = 0;
  virtual void send_byte(uint8_t byte, bool is_data) const = 0;
  virtual void begin() const = 0;

protected:
  const LcdBusActions &actions_;
  const LcdTimer &timer_;
};
