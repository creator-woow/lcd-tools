#include "lcd_gpio_bus.h"

LcdGpioBus::LcdGpioBus(Gpio &gpio, Clock &clock, LcdGpioPins pins)
    : LcdBus(clock), gpio_(gpio), pins_(pins) {}

void LcdGpioBus::begin() {
  gpio_.set_output(pins_.pin_rs);
  gpio_.set_output(pins_.pin_e);
  gpio_.set_output(pins_.pin_d4);
  gpio_.set_output(pins_.pin_d5);
  gpio_.set_output(pins_.pin_d6);
  gpio_.set_output(pins_.pin_d7);
}

void LcdGpioBus::pulse_enable_pin() {
  gpio_.write(pins_.pin_e, false);
  clock_.delay_microseconds(1);
  gpio_.write(pins_.pin_e, true);
  clock_.delay_microseconds(1);
  gpio_.write(pins_.pin_e, false);
  clock_.delay_microseconds(50);
}

void LcdGpioBus::send_high_nibble(uint8_t byte, bool is_data) {
  gpio_.write(pins_.pin_rs, is_data);
  gpio_.write(pins_.pin_d7, byte & 1u << 7);
  gpio_.write(pins_.pin_d6, byte & 1u << 6);
  gpio_.write(pins_.pin_d5, byte & 1u << 5);
  gpio_.write(pins_.pin_d4, byte & 1u << 4);
  pulse_enable_pin();
}
