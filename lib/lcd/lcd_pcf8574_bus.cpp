#include "lcd_pcf8574_bus.h"

LcdPcdf8574Bus::LcdPcdf8574Bus(uint8_t address, I2c &i2c,
                               const LcdBusActions &actions,
                               const LcdTimer &timer)
    : LcdBus(actions, timer), address_(address), i2c_(i2c) {}

void LcdPcdf8574Bus::send_high_nibble(uint8_t byte, bool is_data) {
  constexpr uint8_t register_select_mask = 1u << 0;
  constexpr uint8_t enable_mask = 1u << 2;
  const uint8_t high_nibble_mask = 0b11110000;

  byte &= high_nibble_mask;

  if (is_data) {
    byte |= register_select_mask;
  }

  i2c_.write(address_, byte);
  timer_.delay_microseconds(1);
  i2c_.write(address_, byte | enable_mask);
  timer_.delay_microseconds(1);
  i2c_.write(address_, byte);
}

void LcdPcdf8574Bus::send_byte(uint8_t byte, bool is_data) {
  send_high_nibble(byte, is_data);
  send_high_nibble(byte << 4, is_data);
}

void LcdPcdf8574Bus::begin() { i2c_.begin(); }
