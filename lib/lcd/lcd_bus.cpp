#include "lcd_bus.h"

void GPIOLCDBus::pulse_enable_pin() {
  handlers_.digital_write(pins_.pin_e, false);
  handlers_.delay_microseconds(1);
  handlers_.digital_write(pins_.pin_e, true);
  handlers_.delay_microseconds(1);
  handlers_.digital_write(pins_.pin_e, false);
  handlers_.delay_microseconds(50);
}

void GPIOLCDBus::send_nibble(uint8_t byte, bool is_data) {
  handlers_.digital_write(pins_.pin_rs, is_data);
  handlers_.digital_write(pins_.pin_d7, byte & 0b10000000);
  handlers_.digital_write(pins_.pin_d6, byte & 0b01000000);
  handlers_.digital_write(pins_.pin_d5, byte & 0b00100000);
  handlers_.digital_write(pins_.pin_d4, byte & 0b00010000);
  pulse_enable_pin();
}

void GPIOLCDBus::send_byte(uint8_t byte, bool is_data) {
  handlers_.digital_write(pins_.pin_rs, is_data);
  handlers_.digital_write(pins_.pin_d7, byte & 0b10000000);
  handlers_.digital_write(pins_.pin_d6, byte & 0b01000000);
  handlers_.digital_write(pins_.pin_d5, byte & 0b00100000);
  handlers_.digital_write(pins_.pin_d4, byte & 0b00010000);
  pulse_enable_pin();
  handlers_.digital_write(pins_.pin_d7, byte & 0b00001000);
  handlers_.digital_write(pins_.pin_d6, byte & 0b00000100);
  handlers_.digital_write(pins_.pin_d5, byte & 0b00000010);
  handlers_.digital_write(pins_.pin_d4, byte & 0b00000001);
  pulse_enable_pin();
}

void GPIOLCDBus::init() {
  handlers_.set_output_pin(pins_.pin_rs);
  handlers_.set_output_pin(pins_.pin_e);
  handlers_.set_output_pin(pins_.pin_d4);
  handlers_.set_output_pin(pins_.pin_d5);
  handlers_.set_output_pin(pins_.pin_d6);
  handlers_.set_output_pin(pins_.pin_d7);
}

void I2CLCDBus::send_nibble(uint8_t byte, bool is_data) {}

void I2CLCDBus::send_byte(uint8_t byte, bool is_data) {}

void I2CLCDBus::init() {}
