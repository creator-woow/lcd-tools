#pragma once

using Millisecond = unsigned long;
using Microsecond = unsigned int;

class Clock {
public:
  virtual ~Clock() = default;

  virtual Millisecond milliseconds_now() = 0;
  virtual void delay_milliseconds(Millisecond milliseconds) = 0;
  virtual void delay_microseconds(Microsecond microseconds) = 0;
};