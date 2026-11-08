#include "lcd_bus.h"

LcdBus::LcdBus(const LcdBusActions &actions, const LcdTimer &timer)
    : actions_(actions), timer_(timer) {}
