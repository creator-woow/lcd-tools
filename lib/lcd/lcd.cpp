#include "lcd.h"

Lcd::Lcd(CharacterDisplay &driver) : driver_(driver) {}

void Lcd::init() {}

void Lcd::tick(unsigned long ms_now) {
  for (uint8_t row = 0; row < driver_.rows(); row++) {
    CharacterAnimationState &row_animation = animations_states_[row];
    if (!row_animation.active)
      continue;
    if (ms_now - row_animation.last_tick_time < row_animation.step_delay_ms)
      continue;
    driver_.set_cursor(row, 0);
    if (row_animation.direction == CharacterAnimationDirection::Left) {
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

void Lcd::print_line(const char *text) {
  while (*text) {
    driver_.write(*text);
    text++;
  }
  set_cursor_next_row();
}

void Lcd::print_animated_line(const CharacterAnimationConfig &config,
                              unsigned long ms_now) {
  CharacterAnimationState &row_animation =
      animations_states_[cursor_position_.row];
  row_animation.active = true;
  row_animation.offset = 0;
  row_animation.last_tick_time = ms_now;
  row_animation.text = config.text;
  row_animation.step_delay_ms = config.step_delay_ms;
  row_animation.direction = config.direction;
  row_animation.infinite = config.infinite;
  set_cursor_next_row();
}

void Lcd::set_cursor_next_row() {
  const uint8_t row = cursor_position_.row + 1;
  driver_.set_cursor(row, 0);
}

void Lcd::tick_rigth_direction_animation(
    const CharacterAnimationState &animation) {
  uint8_t text_length = animation.text.length();
  for (uint8_t cell = 0; cell < driver_.columns(); cell++) {
    uint8_t symbol_index =
        text_length - driver_.columns() - animation.offset + cell;
    char symbol = symbol_index < text_length ? animation.text[symbol_index]
                  : animation.infinite       ? animation.text[symbol_index]
                                             : ' ';
    driver_.write(symbol);
  }
}

void Lcd::tick_left_direction_animation(
    const CharacterAnimationState &animation) {
  for (uint8_t cell = 0; cell < driver_.columns(); cell++) {
    uint8_t symbol_index = cell + animation.offset;
    uint8_t text_length = animation.text.length();
    char symbol;
    if (symbol_index < text_length) {
      symbol = animation.text[symbol_index];
    } else if (animation.infinite &&
               symbol_index >
                   text_length -
                       CharacterAnimationConfig::infite_row_gap_width) {
      symbol = animation.text[text_length - animation.offset - cell];
    } else {
      symbol = ' ';
    };
    driver_.write(symbol);
  }
}
