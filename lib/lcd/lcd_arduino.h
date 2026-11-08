#pragma once

#include "lcd.h"
#include "lcd_gpio_bus.h"

extern LcdBusActions arduino_bus_actions;
extern LcdTimer arduino_lcd_timer;

struct ArduinoLcdConfig {
  const LcdBus &bus;
  LcdRowsConfig rows_config{};
};

struct ArduinoGpioLcdConfig {
  LcdGpioPins pins{};
  LcdRowsConfig rows_config{};
};

class ArduinoGpioLcdBus : public LcdGpioBus {
public:
  explicit ArduinoGpioLcdBus(LcdGpioPins pins = {});
};

class ArduinoLcd : public Lcd {
public:
  explicit ArduinoLcd(const ArduinoLcdConfig &config);
};

class ArduinoGpioLcd : public ArduinoLcd, private ArduinoGpioLcdBus {
private:
  static ArduinoGpioLcdBus init_bus(LcdGpioPins pins);

public:
  explicit ArduinoGpioLcd(const ArduinoGpioLcdConfig &config = {});
};
