#pragma once

#include <WString.h>
#include <lcd_bus.h>

using MillisecondsNowHandler = unsigned long (*)();
using DelayMicrosecondsHandler = void (*)(uint64_t microseconds);
using DelayMillisecondsHandler = void (*)(uint64_t milliseconds);

struct LCDHandlers {
  MillisecondsNowHandler milliseconds_now;
  DelayMicrosecondsHandler delay_microseconds;
  DelayMillisecondsHandler delay_milliseconds;
};

enum class LcdCommand : uint8_t {
  ClearDisplay = 0x01,
  ReturnHome = 0x02,
  EntryModeLeft = 0x04,
  EntryModeRight = 0x06,
  DisplayOff = 0x08,
  DisplayOn = 0x0C,
  DisplayOnCursor = 0x0E,
  DisplayOnBlink = 0x0F,
  SetDdramAddress = 0x80,
  FunctionSet8Bit = 0x30,
  FunctionSet4Bit = 0x20,
  FunctionSet4Bit2Line = 0x28
};

enum class LcdAnimationDirection : uint8_t { Left, Rigth };

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
  uint8_t starts_addresses[max_rows] = {0x00, 0x40, 0x00, 0x00};
};

struct LcdCursorPosition {
  uint8_t row;
  uint8_t cell;
};

struct LcdConfig {
  const LcdRowsConfig rows_config;
  LCDBus &bus;
  const LCDHandlers &handlers;
};

class Lcd {
public:
  explicit Lcd();
  explicit Lcd(LcdConfig config);
  void print_line(const char *text);
  void print_animated_line(const LcdAnimationConfig &rows_config);
  void tick();

private:
  void set_cursor(LcdCursorPosition position);
  void tick_left_direction_animation(const LcdAnimationState &animation);
  void tick_rigth_direction_animation(const LcdAnimationState &animation);
  void set_cursor_next_row();
  void clear();
  void init();
  void send_command(LcdCommand command);
  void send_command(uint8_t command);
  void send_nibble_command(LcdCommand command);
  void send_data(uint8_t data);

  const LcdRowsConfig rows_config_;
  const LCDHandlers handlers_;
  LCDBus &bus_;
  LcdCursorPosition cursor_position_{0, 0};
  LcdAnimationState animations_states_[LcdRowsConfig::max_rows];
};