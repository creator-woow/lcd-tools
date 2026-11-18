#pragma once

#include "clock.h"
#include "i2c.h"

#include "lcd_bus.h"

class LcdPcf8574Bus : public LcdBus {
public:
  LcdPcf8574Bus(uint8_t address, I2c &i2c, Clock &clock);
  void send_high_nibble(uint8_t byte, bool is_data) override;
  void begin() override;

private:
  uint8_t address_;
  I2c &i2c_;
};
