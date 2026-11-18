#pragma once

#include "i2c.h"

class WireI2c : public I2c {
public:
  void begin() override;
  I2cStatus write(uint8_t address, uint8_t byte) override;
};
