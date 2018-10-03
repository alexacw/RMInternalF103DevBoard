#include "ch.h"
#include "ch.hpp"
#include "hal.h"

#define PWM1_FREQ 100000 /* 100kHz PWM clock frequency.   */
#define PWM1_PERIOD 1000 /* Initial PWM period 10ms       */

namespace PWM_Ctrl
{
void startBreathLight();
void stopBreathLight();
} // namespace PWM_Ctrl
