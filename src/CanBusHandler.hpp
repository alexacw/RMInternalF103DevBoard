
#include "ch.hpp"
#include "hal.h"
#include "chprintf.h"

namespace CanBusHandler
{
extern int32_t receiveCount;
extern int16_t current_1;
extern CANRxFrame f;
void caninit(CANDriver *canp);
}; // namespace CanBusHandler