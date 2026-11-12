#include <Wire.h>

#include "i2c_wire.h"

void WireI2c::begin() { Wire.begin(); }

I2cStatus WireI2c::write(int address, uint8_t byte) {
  Wire.beginTransmission(address);
  Wire.write(byte);
  I2cStatus output_code = I2cStatus(Wire.endTransmission());
  return output_code;
}
