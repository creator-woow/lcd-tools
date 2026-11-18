#pragma once

#include "character_animation.h"
#include "character_display.h"

struct LcdGeometry {};

class Lcd {
public:
  Lcd(CharacterDisplay &driver);
  void init();
  void tick(unsigned long ms_now);
  void print_line(const char *text);
  void print_animated_line(const CharacterAnimationConfig &animation_config,
                           unsigned long ms_now);
  void set_cursor_next_row();

private:
  static constexpr uint8_t max_rows = 4;

  CharacterDisplay &driver_;
  CharacterAnimationState animations_states_[Lcd::max_rows];
  CharacterDisplayCursorPosition cursor_position_{0, 0};

  void tick_left_direction_animation(const CharacterAnimationState &animation);
  void tick_rigth_direction_animation(const CharacterAnimationState &animation);
};
