#include <Arduino.h>

#include "lcd.h"

enum class LcdCommand : uint8_t {
  ClearDisplay        = 0x01,
  ReturnHome          = 0x02,
  EntryModeLeft       = 0x04,
  EntryModeRight      = 0x06,
  DisplayOff          = 0x08,
  DisplayOn           = 0x0C,
  DisplayOnCursor     = 0x0E,
  DisplayOnBlink      = 0x0F,
  SetDdramAddress     = 0x80,
  FunctionSet8Bit     = 0x30,
  FunctionSet4Bit     = 0x20,
  FunctionSet4Bit2Line = 0x28
};

Lcd::Lcd(const LcdConfig& config) : config_(config) {
  init();
}

void Lcd::print_line(const char* text) {
  while(*text) {
    send_data(*text);
    text++;
  }
  set_cursor_next_row();
}

void Lcd::print_animated_line(const LcdAnimationConfig& config) {
  LcdAnimationState& row_animation = animations_states_[cursor_position_.row];
  row_animation.active = true;
  row_animation.offset = 0;
  row_animation.last_tick_time = millis();
  row_animation.text = config.text;
  row_animation.step_delay_ms = config.step_delay_ms;
  row_animation.direction = config.direction;
  set_cursor_next_row();
}

void Lcd::tick() {
  unsigned long ms_now = millis();
  
  for (uint8_t row = 0; row < config_.rows_config.amount; row++) {
    LcdAnimationState& row_animation = animations_states_[row];
    if (!row_animation.active) continue;
    if (ms_now - row_animation.last_tick_time <  row_animation.step_delay_ms) continue;
    set_cursor({ .row = row, .cell = 0 });
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

void Lcd::tick_left_direction_animation(const LcdAnimationState& animation) {
  for (uint8_t cell = 0; cell < config_.rows_config.width; cell++) {
    uint8_t symbol_index = cell + animation.offset;
    uint8_t text_length = animation.text.length();
    char symbol;
    if (symbol_index < text_length) {
      symbol = animation.text[symbol_index];
    } else if (animation.infinite && symbol_index > text_length - LcdAnimationConfig::infite_row_gap_width) {
      symbol = animation.text[text_length - animation.offset - cell];
    } else {
      symbol = ' ';
    };
    send_data(symbol);
  }
}

void Lcd::tick_rigth_direction_animation(const LcdAnimationState& animation) {
  uint8_t text_length = animation.text.length();
  for (uint8_t cell = 0; cell < config_.rows_config.width; cell++) {
    uint8_t symbol_index = text_length - config_.rows_config.width - animation.offset+ cell;
    char symbol = symbol_index < text_length
      ? animation.text[symbol_index]
      : animation.infinite
        ? animation.text[symbol_index]
        : ' ';
    send_data(symbol);
  }
}

void Lcd::pulse_enable_pin() {
  digitalWrite(config_.pins.pin_e, LOW);
  delayMicroseconds(1);
  digitalWrite(config_.pins.pin_e, HIGH);
  delayMicroseconds(1);
  digitalWrite(config_.pins.pin_e, LOW);
  delayMicroseconds(50);
}

void Lcd::init() {
  pinMode(config_.pins.pin_rs, OUTPUT);
  pinMode(config_.pins.pin_e, OUTPUT);
  pinMode(config_.pins.pin_d4, OUTPUT);
  pinMode(config_.pins.pin_d5, OUTPUT);
  pinMode(config_.pins.pin_d6, OUTPUT);
  pinMode(config_.pins.pin_d7, OUTPUT);
  delay(70);
  send_nibble_command(LcdCommand::FunctionSet8Bit);
  delay(70);
  send_nibble_command(LcdCommand::FunctionSet8Bit);
  delayMicroseconds(150);
  send_nibble_command(LcdCommand::FunctionSet8Bit);
  delayMicroseconds(150);
  send_nibble_command(LcdCommand::FunctionSet4Bit);
  delay(70);
  send_command(LcdCommand::FunctionSet4Bit2Line);
  delay(70);
  send_command(LcdCommand::DisplayOff);
  delay(70);
  clear();
  delay(70);
  send_command(LcdCommand::EntryModeRight);
  delay(70);
  send_command(LcdCommand::DisplayOn);
  delay(70);
};


void Lcd::send_nibble(uint8_t byte, bool is_data) {
  digitalWrite(config_.pins.pin_rs, is_data ? HIGH : LOW);
  digitalWrite(config_.pins.pin_d7, byte & 0b10000000);
  digitalWrite(config_.pins.pin_d6, byte & 0b01000000);
  digitalWrite(config_.pins.pin_d5, byte & 0b00100000);
  digitalWrite(config_.pins.pin_d4, byte & 0b00010000);
  pulse_enable_pin();
}

void Lcd::send_byte(uint8_t byte, bool is_data) {
  digitalWrite(config_.pins.pin_rs, is_data ? HIGH : LOW);
  digitalWrite(config_.pins.pin_d7, byte & 0b10000000);
  digitalWrite(config_.pins.pin_d6, byte & 0b01000000);
  digitalWrite(config_.pins.pin_d5, byte & 0b00100000);
  digitalWrite(config_.pins.pin_d4, byte & 0b00010000);
  pulse_enable_pin();
  digitalWrite(config_.pins.pin_d7, byte & 0b00001000);
  digitalWrite(config_.pins.pin_d6, byte & 0b00000100);
  digitalWrite(config_.pins.pin_d5, byte & 0b00000010);
  digitalWrite(config_.pins.pin_d4, byte & 0b00000001);
  pulse_enable_pin();
}

void Lcd::send_command(uint8_t command) {
  send_byte(command, false);
}

void Lcd::send_command(LcdCommand command) {
  send_byte(static_cast<uint8_t>(command), false);
}

void Lcd::send_nibble_command(LcdCommand command) {
  send_nibble(static_cast<uint8_t>(command), false);
}

void Lcd::send_data(uint8_t data) {
  send_byte(data, true);
}

void Lcd::set_cursor_next_row() {
  const uint8_t row = cursor_position_.row + 1;
  set_cursor({ .row = row, .cell = 0});
}

void Lcd::clear() {
  send_command(LcdCommand::ClearDisplay);
  delay(5);
}

void Lcd::set_cursor(LcdCursorPosition cursor_position) {
  if (cursor_position.row >= config_.rows_config.amount || cursor_position.cell >= config_.rows_config.width) return;
  uint8_t row_address = config_.rows_config.starts_addresses[cursor_position.row];
  uint8_t command = static_cast<uint8_t>(LcdCommand::SetDdramAddress) | (row_address + cursor_position.cell);
  send_command(command);
  cursor_position_ = cursor_position;
  delay(5);
}

