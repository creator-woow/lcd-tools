#pragma once
#include <stdint.h>

using DigitalWriteHandler = void (*)(uint8_t pin, bool high);
using SetOutputPinModeHandler = void (*)(uint8_t pin);
using SetInputPinModeHandler = void (*)(uint8_t pin);
using DelayMicroseconds = void (*)(uint64_t microseconds);

struct LCDBusHandlers {
  DigitalWriteHandler digital_write;
  SetOutputPinModeHandler set_output_pin;
  SetInputPinModeHandler set_input_pin;
  DelayMicroseconds delay_microseconds;
};

class LCDBus {
public:
  virtual ~LCDBus() = 0;
  virtual void send_nibble(uint8_t byte, bool is_data) = 0;
  virtual void send_byte(uint8_t byte, bool is_data) = 0;
  virtual void init() = 0;

protected:
  LCDBusHandlers &handlers_;
};

struct LCDGPIOPins {
  uint8_t pin_e = 11;
  uint8_t pin_rs = 12;
  uint8_t pin_d4 = 2;
  uint8_t pin_d5 = 3;
  uint8_t pin_d6 = 4;
  uint8_t pin_d7 = 5;
};

class GPIOLCDBus : public LCDBus {
public:
  void send_nibble(uint8_t byte, bool is_data) override;
  void send_byte(uint8_t byte, bool is_data) override;
  void init() override;

private:
  void pulse_enable_pin();
  LCDGPIOPins pins_;
};

class I2CLCDBus : public LCDBus {
public:
  void send_nibble(uint8_t byte, bool is_data) override;
  void send_byte(uint8_t byte, bool is_data) override;
  void init() override;
};
