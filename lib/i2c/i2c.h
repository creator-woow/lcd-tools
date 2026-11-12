#pragma once

#include <Wire.h>

enum class I2cStatus : uint8_t {
  Success = 0,
  BufferOverflow = 1,
  AddressError = 2,
  DataError = 3,
  OtherError = 4,
  Timeout = 5
};

class I2c {
public:
  virtual ~I2c() = default;

  virtual void begin() = 0;
  virtual I2cStatus write(int address, uint8_t byte) = 0;
};