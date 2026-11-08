#pragma once

#include "lcd_bus.h"

class LcdI2cBus : public LcdBus {
public:
  void send_nibble(uint8_t byte, bool is_data) const override;
  void send_byte(uint8_t byte, bool is_data) const override;
  void init() const override;
};
