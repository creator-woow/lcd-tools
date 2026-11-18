#pragma once

#include "gpio.h"

#include "lcd_bus.h"

struct LcdGpioPins {
  PinNumber pin_e = 11;
  PinNumber pin_rs = 12;
  PinNumber pin_d4 = 2;
  PinNumber pin_d5 = 3;
  PinNumber pin_d6 = 4;
  PinNumber pin_d7 = 5;
};

class LcdGpioBus : public LcdBus {
public:
  explicit LcdGpioBus(Gpio &gpio, Clock &clock, LcdGpioPins pins);

  void begin() override;
  void send_high_nibble(uint8_t byte, bool is_data) override;

private:
  void pulse_enable_pin();
  Gpio &gpio_;
  const LcdGpioPins pins_;
};