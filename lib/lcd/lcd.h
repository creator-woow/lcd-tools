#pragma once
#include <Arduino.h>

enum class LcdCommand : uint8_t;
enum class LcdAnimationDirection : uint8_t;

struct LcdPins {
  uint8_t pin_e;
  uint8_t pin_rs;
  uint8_t pin_d4;
  uint8_t pin_d5;
  uint8_t pin_d6;
  uint8_t pin_d7;
};

enum class LcdAnimationDirection : uint8_t {
  Left,
  Rigth
};

struct LcdAnimationConfig {
  static constexpr uint8_t infite_row_gap_width = 6;
  String text;
  LcdAnimationDirection direction = LcdAnimationDirection::Left;
  bool infinite = false;
  uint16_t step_delay_ms = 500;
};

struct LcdAnimationState : LcdAnimationConfig {
  bool active = false;
  uint8_t offset = 0;
  unsigned long last_tick_time = 0;
};

struct LcdRowsConfig {
  static constexpr uint8_t max_rows = 4;
  uint8_t amount = 2;
  uint8_t width = 16;
  uint8_t starts_addresses[max_rows] = { 0x00, 0x40, 0x00, 0x00 };
};

struct LcdCursorPosition {
  uint8_t row;
  uint8_t cell;
};

struct LcdConfig {
  LcdPins pins;
  LcdRowsConfig rows_config;
};

class Lcd {
public:
  Lcd() = default;
  explicit Lcd(const LcdConfig& config);

  void print_line(const char* text);
  void print_animated_line(const LcdAnimationConfig& config);
  void tick();

private:
  void pulse_enable_pin();
  void send_nibble(uint8_t byte, bool is_data);
  void send_byte(uint8_t byte, bool is_data);
  void send_command(LcdCommand command);
  void send_command(uint8_t command);
  void send_nibble_command(LcdCommand command);
  void send_data(uint8_t data);
  void set_cursor(LcdCursorPosition position);
  void tick_left_direction_animation(const LcdAnimationState& animation);
  void tick_rigth_direction_animation(const LcdAnimationState& animation);
  void set_cursor_next_row();
  void clear();
  void init();

  LcdConfig config_;
  LcdCursorPosition cursor_position_{0, 0};
  LcdAnimationState animations_states_[LcdRowsConfig::max_rows];
};