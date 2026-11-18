#pragma once

#include <stdint.h>

#include "clock.h"

class LcdBus {
public:
  explicit LcdBus(Clock &clock);
  virtual ~LcdBus() = default;

  virtual void send_high_nibble(uint8_t byte, bool is_data) = 0;
  virtual void begin() = 0;

  void send_byte(uint8_t byte, bool is_data) {
    send_high_nibble(byte, is_data);
    send_high_nibble(byte << 4, is_data);
  }

protected:
  Clock &clock_;
};
