#pragma once

#include <stdint.h>

struct CharacterDisplayCursorPosition {
  uint8_t row;
  uint8_t column;
};

class CharacterDisplay {
public:
  virtual ~CharacterDisplay() = default;

  virtual void init() = 0;
  virtual uint8_t rows() = 0;
  virtual uint8_t columns() = 0;
  virtual void clear() = 0;
  virtual void set_cursor(uint8_t row, uint8_t cell) = 0;
  virtual void write(uint8_t byte) = 0;
};
