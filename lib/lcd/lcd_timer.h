#pragma once

using MillisecondsNowHandler = unsigned long (*)();
using DelayMicrosecondsHandler = void (*)(unsigned int microseconds);
using DelayMillisecondsHandler = void (*)(unsigned long milliseconds);

struct LcdTimer {
  MillisecondsNowHandler milliseconds_now;
  DelayMicrosecondsHandler delay_microseconds;
  DelayMillisecondsHandler delay_milliseconds;
};