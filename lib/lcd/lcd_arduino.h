#pragma once

#include "i2c_wire.h"

#include "lcd.h"
#include "lcd_gpio_bus.h"
#include "lcd_pcf8574_bus.h"

extern LcdBusActions arduino_bus_actions;
extern LcdTimer arduino_lcd_timer;

struct ArduinoLcdConfig {
  LcdBus &bus;
  LcdRowsConfig rows_config{};
};

struct ArduinoGpioLcdConfig {
  LcdGpioPins pins{};
  LcdRowsConfig rows_config{};
};

extern WireI2c arduino_default_i2c;

struct ArduinoPcf8574LcdConfig {
  uint8_t address = 0x27;
  I2c &i2c = arduino_default_i2c;
  LcdRowsConfig rows_config{};
};

class ArduinoGpioLcdBus : public LcdGpioBus {
public:
  explicit ArduinoGpioLcdBus(LcdGpioPins pins = {});
};

class ArduinoPcf8574LcdBus : public LcdPcdf8574Bus {
public:
  explicit ArduinoPcf8574LcdBus(uint8_t address, I2c &i2c);
};

class ArduinoLcd : public Lcd {
public:
  explicit ArduinoLcd(const ArduinoLcdConfig &config);
};

class ArduinoGpioLcd : private ArduinoGpioLcdBus, public ArduinoLcd {
public:
  explicit ArduinoGpioLcd(const ArduinoGpioLcdConfig &config = {});
};

class ArduinoPcf8574Lcd : private ArduinoPcf8574LcdBus, public ArduinoLcd {
public:
  explicit ArduinoPcf8574Lcd(const ArduinoPcf8574LcdConfig &config = {});
};
