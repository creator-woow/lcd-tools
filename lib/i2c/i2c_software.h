#include "i2c.h"

class SoftwareI2c : public I2c {
public:
  I2cStatus write(int address, uint8_t byte) override;
};
