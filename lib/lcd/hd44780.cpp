#include "hd44780.h"

namespace commands_bits {
constexpr uint8_t clear_display = 1u << 0;
constexpr uint8_t return_home = 1u << 1;

constexpr uint8_t entry_mode_set = 1u << 2;
constexpr uint8_t entry_mode_right = 1 << 1;
constexpr uint8_t entry_mode_shift = 1 << 0;

constexpr uint8_t display_control = 1u << 3;
constexpr uint8_t display_on = 1u << 2;
constexpr uint8_t cursor_on = 1u << 1;
constexpr uint8_t blink_on = 1u << 0;

constexpr uint8_t shift_control = 1u << 4;
constexpr uint8_t shift_screen = 1u << 3;
constexpr uint8_t shift_cursor_right = 1u << 2;

constexpr uint8_t function_set = 1u << 5;
constexpr uint8_t function_set_eight_bits = 1u << 4;
constexpr uint8_t function_set_two_rows = 1u << 3;
constexpr uint8_t function_set_5x10_column = 1u << 2;

constexpr uint8_t set_cgram_address = 1u << 6;
constexpr uint8_t set_ddram_address = 1u << 7;

} // namespace commands_bits

Hd44780::Hd44780(LcdBus &bus, Clock &clock, const Hd44780Geometry geometry)
    : bus_(bus), clock_(clock), geometry_(geometry) {}

void Hd44780::send_command(uint8_t command) {
  bus_.send_byte(uint8_t(command), false);
}

void Hd44780::send_nibble_command(uint8_t command) {
  bus_.send_high_nibble(uint8_t(command), false);
}

void Hd44780::send_data(uint8_t byte) { bus_.send_byte(byte, true); }

constexpr uint8_t Hd44780::flag_if(bool condition, uint8_t byte) {
  return condition ? byte : 0;
}

constexpr uint8_t
Hd44780::entry_mode_command(CursorDirection direction = CursorDirection::Right,
                            DisplayShift shift = DisplayShift::None) {
  return commands_bits::entry_mode_set |
         flag_if(direction == CursorDirection::Right,
                 commands_bits::entry_mode_right) |
         flag_if(shift == DisplayShift::Shift, commands_bits::entry_mode_shift);
}

constexpr uint8_t Hd44780::display_control_command(bool display = false,
                                                   bool cursor = false,
                                                   bool blink = false) {
  return commands_bits::display_control |
         flag_if(display, commands_bits::display_on) |
         flag_if(cursor, commands_bits::cursor_on) |
         flag_if(blink, commands_bits::blink_on);
}

constexpr uint8_t
Hd44780::function_set_command(DataLines data_lines = DataLines::Eight,
                              DisplayRows rows_amount = DisplayRows::Two,
                              ColumnSize column_size = ColumnSize::Default) {
  return commands_bits::function_set |
         flag_if(data_lines == DataLines::Eight,
                 commands_bits::function_set_eight_bits) |
         flag_if(rows_amount == DisplayRows::Two,
                 commands_bits::function_set_two_rows) |
         flag_if(column_size == ColumnSize::Big,
                 commands_bits::function_set_5x10_column);
}

constexpr uint8_t Hd44780::set_ddram_command(uint8_t address) {
  return commands_bits::set_ddram_address | (address & 0b0111'1111);
}

void Hd44780::init() {
  bus_.begin();
  clock_.delay_milliseconds(70);
  send_nibble_command(function_set_command());
  clock_.delay_milliseconds(70);
  send_nibble_command(function_set_command());
  clock_.delay_microseconds(150);
  send_nibble_command(function_set_command());
  clock_.delay_microseconds(150);
  send_nibble_command(function_set_command(DataLines::Four));
  clock_.delay_milliseconds(70);
  send_command(function_set_command(DataLines::Four, DisplayRows::Two));
  clock_.delay_milliseconds(70);
  send_command(display_control_command(false));
  clock_.delay_milliseconds(70);
  clear();
  clock_.delay_milliseconds(70);
  send_command(entry_mode_command(CursorDirection::Right));
  clock_.delay_milliseconds(70);
  send_command(display_control_command(true));
  clock_.delay_milliseconds(70);
}

uint8_t Hd44780::rows() { return geometry_.rows; }

uint8_t Hd44780::columns() { return geometry_.columns; }

void Hd44780::clear() {
  send_command(commands_bits::clear_display);
  clock_.delay_milliseconds(5);
}

void Hd44780::set_cursor(uint8_t row, uint8_t column) {
  if (row >= geometry_.rows || column >= geometry_.columns)
    return;
  uint8_t row_address = geometry_.starts_addresses[row];
  uint8_t command = set_ddram_command(row_address + column);
  send_command(command);
  clock_.delay_milliseconds(5);
}

void Hd44780::write(uint8_t byte) { send_data(byte); }
