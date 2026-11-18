#pragma once

#include <WString.h>
#include <stdint.h>

enum class CharacterAnimationDirection : uint8_t { Left, Rigth };

struct CharacterAnimationConfig {
  static constexpr uint8_t infite_row_gap_width = 6;
  String text;
  CharacterAnimationDirection direction = CharacterAnimationDirection::Left;
  bool infinite = false;
  uint16_t step_delay_ms = 500;
};

struct CharacterAnimationState : CharacterAnimationConfig {
  bool active = false;
  uint8_t offset = 0;
  unsigned long last_tick_time = 0;
};
