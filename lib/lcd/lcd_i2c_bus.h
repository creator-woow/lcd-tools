#pragma once

#include "i2c.h"

#include "lcd_bus.h"

class LcdI2cBus : public LcdBus {
public:
  LcdI2cBus(uint8_t address, I2c &i2c_realization, const LcdBusActions &actions,
            const LcdTimer &timer);
  void send_nibble(uint8_t byte, bool is_data) const override;
  void send_byte(uint8_t byte, bool is_data) const override;
  void begin() const override;

private:
  uint8_t address_;
  I2c &i2c_;
};
