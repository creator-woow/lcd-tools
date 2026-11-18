#include "lcd_pcf8574_bus.h"

LcdPcf8574Bus::LcdPcf8574Bus(uint8_t address, I2c &i2c, Clock &clock)
    : LcdBus(clock), address_(address), i2c_(i2c) {}

void LcdPcf8574Bus::send_high_nibble(uint8_t byte, bool is_data) {
  constexpr uint8_t rs_data_mask = 1u << 0;
  constexpr uint8_t enable_e_mask = 1u << 2;
  constexpr uint8_t enable_highlight_mask = 1u << 3;
  constexpr const uint8_t high_nibble_mask = 0b11110000;

  byte &= high_nibble_mask;

  if (is_data) {
    byte |= rs_data_mask;
  }

  i2c_.write(address_, byte);
  clock_.delay_microseconds(1);
  i2c_.write(address_, byte | enable_e_mask);
  clock_.delay_microseconds(1);
  i2c_.write(address_, byte);
}

void LcdPcf8574Bus::begin() { i2c_.begin(); }
