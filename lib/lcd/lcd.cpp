#include "lcd.h"

Lcd::Lcd(LcdConfig config)
    : rows_config_(config.rows_config), bus_(config.bus),
      handlers_(config.handlers) {
  init();
}

void Lcd::print_line(const char *text) {
  while (*text) {
    send_data(*text);
    text++;
  }
  set_cursor_next_row();
}

void Lcd::print_animated_line(const LcdAnimationConfig &config) {
  LcdAnimationState &row_animation = animations_states_[cursor_position_.row];
  row_animation.active = true;
  row_animation.offset = 0;
  row_animation.last_tick_time = handlers_.milliseconds_now();
  row_animation.text = config.text;
  row_animation.step_delay_ms = config.step_delay_ms;
  row_animation.direction = config.direction;
  set_cursor_next_row();
}

void Lcd::tick() {
  unsigned long ms_now = handlers_.milliseconds_now();

  for (uint8_t row = 0; row < rows_config_.amount; row++) {
    LcdAnimationState &row_animation = animations_states_[row];
    if (!row_animation.active)
      continue;
    if (ms_now - row_animation.last_tick_time < row_animation.step_delay_ms)
      continue;
    set_cursor({.row = row, .cell = 0});
    if (row_animation.direction == LcdAnimationDirection::Left) {
      tick_left_direction_animation(row_animation);
    } else {
      tick_rigth_direction_animation(row_animation);
    }
    row_animation.offset++;
    if (row_animation.offset > row_animation.text.length()) {
      row_animation.offset = 0;
    }
    row_animation.last_tick_time = ms_now;
  }
}

void Lcd::tick_left_direction_animation(const LcdAnimationState &animation) {
  for (uint8_t cell = 0; cell < rows_config_.width; cell++) {
    uint8_t symbol_index = cell + animation.offset;
    uint8_t text_length = animation.text.length();
    char symbol;
    if (symbol_index < text_length) {
      symbol = animation.text[symbol_index];
    } else if (animation.infinite &&
               symbol_index >
                   text_length - LcdAnimationConfig::infite_row_gap_width) {
      symbol = animation.text[text_length - animation.offset - cell];
    } else {
      symbol = ' ';
    };
    send_data(symbol);
  }
}

void Lcd::tick_rigth_direction_animation(const LcdAnimationState &animation) {
  uint8_t text_length = animation.text.length();
  for (uint8_t cell = 0; cell < rows_config_.width; cell++) {
    uint8_t symbol_index =
        text_length - rows_config_.width - animation.offset + cell;
    char symbol = symbol_index < text_length ? animation.text[symbol_index]
                  : animation.infinite       ? animation.text[symbol_index]
                                             : ' ';
    send_data(symbol);
  }
}

void Lcd::send_command(uint8_t command) { bus_.send_byte(command, false); }

void Lcd::send_command(LcdCommand command) {
  bus_.send_byte(static_cast<uint8_t>(command), false);
}

void Lcd::send_nibble_command(LcdCommand command) {
  bus_.send_nibble(static_cast<uint8_t>(command), false);
}

void Lcd::send_data(uint8_t data) { bus_.send_byte(data, true); }

void Lcd::init() {
  bus_.init();
  handlers_.delay_milliseconds(70);
  send_nibble_command(LcdCommand::FunctionSet8Bit);
  handlers_.delay_milliseconds(70);
  send_nibble_command(LcdCommand::FunctionSet8Bit);
  handlers_.delay_microseconds(150);
  send_nibble_command(LcdCommand::FunctionSet8Bit);
  handlers_.delay_microseconds(150);
  send_nibble_command(LcdCommand::FunctionSet4Bit);
  handlers_.delay_milliseconds(70);
  send_command(LcdCommand::FunctionSet4Bit2Line);
  handlers_.delay_milliseconds(70);
  send_command(LcdCommand::DisplayOff);
  handlers_.delay_milliseconds(70);
  clear();
  handlers_.delay_milliseconds(70);
  send_command(LcdCommand::EntryModeRight);
  handlers_.delay_milliseconds(70);
  send_command(LcdCommand::DisplayOn);
  handlers_.delay_milliseconds(70);
}

void Lcd::set_cursor_next_row() {
  const uint8_t row = cursor_position_.row + 1;
  set_cursor({.row = row, .cell = 0});
}

void Lcd::clear() {
  send_command(LcdCommand::ClearDisplay);
  handlers_.delay_milliseconds(5);
}

void Lcd::set_cursor(LcdCursorPosition cursor_position) {
  if (cursor_position.row >= rows_config_.amount ||
      cursor_position.cell >= rows_config_.width)
    return;
  uint8_t row_address = rows_config_.starts_addresses[cursor_position.row];
  uint8_t command = static_cast<uint8_t>(LcdCommand::SetDdramAddress) |
                    (row_address + cursor_position.cell);
  send_command(command);
  cursor_position_ = cursor_position;
  handlers_.delay_milliseconds(5);
}
