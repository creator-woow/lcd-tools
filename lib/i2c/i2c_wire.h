#include "i2c.h"

class WireI2c : public I2c {
public:
  void begin() override;
  I2cStatus write(int address, uint8_t byte) override;
};
