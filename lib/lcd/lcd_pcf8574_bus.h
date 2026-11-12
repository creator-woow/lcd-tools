#pragma once

#include "i2c.h"

#include "lcd_bus.h"

class LcdPcdf8574Bus : public LcdBus {
public:
  LcdPcdf8574Bus(uint8_t address, I2c &i2c, const LcdBusActions &actions,
                 const LcdTimer &timer);
  void send_high_nibble(uint8_t byte, bool is_data) const override;
  void send_byte(uint8_t byte, bool is_data) const override;
  void begin() const override;

private:
  uint8_t address_;
  I2c &i2c_;
};
