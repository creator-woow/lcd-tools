#pragma once

#include "lcd_bus.h"

struct LcdGpioPins {
  uint8_t pin_e = 11;
  uint8_t pin_rs = 12;
  uint8_t pin_d4 = 2;
  uint8_t pin_d5 = 3;
  uint8_t pin_d6 = 4;
  uint8_t pin_d7 = 5;
};

class LcdGpioBus : public LcdBus {
public:
  explicit LcdGpioBus(const LcdBusActions &actions, const LcdTimer &timer,
                      LcdGpioPins pins);

  void begin() override;
  void send_high_nibble(uint8_t byte, bool is_data) override;
  void send_byte(uint8_t byte, bool is_data) override;

private:
  void pulse_enable_pin();
  const LcdGpioPins pins_;
};