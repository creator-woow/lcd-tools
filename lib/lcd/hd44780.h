#pragma once

#include "character_display.h"
#include "clock.h"
#include "lcd_bus.h"

struct Hd44780Geometry {
  static constexpr uint8_t max_rows = 4;
  uint8_t rows = 2;
  uint8_t columns = 16;
  uint8_t starts_addresses[Hd44780Geometry::max_rows] = {0x00, 0x40, 0x00,
                                                         0x00};
};

enum class CursorDirection : uint8_t { Left, Right };

enum class DisplayShift : uint8_t { Shift, None };

enum class DataLines : uint8_t { Eight, Four };

enum class DisplayRows : uint8_t { One, Two };

enum class ColumnSize : uint8_t { Default, Big };

class Hd44780 : public CharacterDisplay {
public:
  Hd44780(LcdBus &bus, Clock &clock, const Hd44780Geometry geometry = {});
  Hd44780(const Hd44780 &) = delete;
  Hd44780 &operator=(const Hd44780 &) = delete;

  void init() override;
  uint8_t rows() override;
  uint8_t columns() override;
  void clear() override;
  void set_cursor(uint8_t row, uint8_t cell) override;
  void write(uint8_t byte) override;

private:
  void send_command(uint8_t command);
  void send_nibble_command(uint8_t command);
  void send_data(uint8_t data);
  constexpr uint8_t flag_if(bool condition, uint8_t byte);
  constexpr uint8_t entry_mode_command(CursorDirection direction,
                                       DisplayShift shift);
  constexpr uint8_t display_control_command(bool display, bool cursor,
                                            bool blink);
  constexpr uint8_t shift_control_command(DisplayShift shift_screen,
                                          CursorDirection cursor_direction);
  constexpr uint8_t function_set_command(DataLines data_lines,
                                         DisplayRows rows_amount,
                                         ColumnSize column_size);
  constexpr uint8_t set_ddram_command(uint8_t address);

  LcdBus &bus_;
  Clock &clock_;
  const Hd44780Geometry geometry_;
};
