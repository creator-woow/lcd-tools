#include "lcd_gpio_bus.h"

LcdGpioBus::LcdGpioBus(const LcdBusActions &actions, const LcdTimer &timer,
                       const LcdGpioPins pins)
    : LcdBus(actions, timer), pins_(pins) {}

void LcdGpioBus::init() const {
  actions_.set_output_pin(pins_.pin_rs);
  actions_.set_output_pin(pins_.pin_e);
  actions_.set_output_pin(pins_.pin_d4);
  actions_.set_output_pin(pins_.pin_d5);
  actions_.set_output_pin(pins_.pin_d6);
  actions_.set_output_pin(pins_.pin_d7);
}

void LcdGpioBus::pulse_enable_pin() const {
  actions_.digital_write(pins_.pin_e, false);
  timer_.delay_microseconds(1);
  actions_.digital_write(pins_.pin_e, true);
  timer_.delay_microseconds(1);
  actions_.digital_write(pins_.pin_e, false);
  timer_.delay_microseconds(50);
}

void LcdGpioBus::send_nibble(uint8_t byte, bool is_data) const {
  actions_.digital_write(pins_.pin_rs, is_data);
  actions_.digital_write(pins_.pin_d7, byte & 0b10000000);
  actions_.digital_write(pins_.pin_d6, byte & 0b01000000);
  actions_.digital_write(pins_.pin_d5, byte & 0b00100000);
  actions_.digital_write(pins_.pin_d4, byte & 0b00010000);
  pulse_enable_pin();
}

void LcdGpioBus::send_byte(uint8_t byte, bool is_data) const {
  actions_.digital_write(pins_.pin_rs, is_data);
  actions_.digital_write(pins_.pin_d7, byte & 0b10000000);
  actions_.digital_write(pins_.pin_d6, byte & 0b01000000);
  actions_.digital_write(pins_.pin_d5, byte & 0b00100000);
  actions_.digital_write(pins_.pin_d4, byte & 0b00010000);
  pulse_enable_pin();
  actions_.digital_write(pins_.pin_d7, byte & 0b00001000);
  actions_.digital_write(pins_.pin_d6, byte & 0b00000100);
  actions_.digital_write(pins_.pin_d5, byte & 0b00000010);
  actions_.digital_write(pins_.pin_d4, byte & 0b00000001);
  pulse_enable_pin();
}
