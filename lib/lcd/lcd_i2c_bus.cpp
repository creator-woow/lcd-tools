#include "lcd_i2c_bus.h"

LcdI2cBus::LcdI2cBus(uint8_t address, I2c &i2c_realization,
                     const LcdBusActions &actions, const LcdTimer &timer)
    : LcdBus(actions, timer), address_(address), i2c_(i2c_realization) {}

void LcdI2cBus::send_nibble(uint8_t byte, bool is_data) const {
  i2c_.write(address_, byte);
}

void LcdI2cBus::send_byte(uint8_t byte, bool is_data) const {
  i2c_.write(address_, byte);
}

void LcdI2cBus::begin() const { i2c_.begin(); }
