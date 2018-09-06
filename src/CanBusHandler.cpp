
#include "ch.hpp"
#include "hal.h"
#include "chprintf.h"

namespace CanBusHandler
{
/**
   * @brief  
   * Internal loopback mode, 500KBaud, automatic wakeup, automatic recover
   * from abort mode.
   * See section 22.7.7 on the STM32 reference manual.
   */
static const CANConfig cancfg = {
    CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
    CAN_BTR_LBKM | CAN_BTR_SJW(0) | CAN_BTR_TS2(1) |
        CAN_BTR_TS1(8) | CAN_BTR_BRP(6)};

/**
 * @brief it will not be called again until the received
 * frames queue has been completely emptied. It
 * is not called for each received frame. It is
 * responsibility of the application to empty the queue by
 * repeatedly invoking @p canTryReceiveI().
 * 
 * @param canp 
 * @param flags 
 */
void rxCB(CANDriver *canp, uint32_t flags){
    canTryReceiveI(canp,)
};
void txCB(CANDriver *canp, uint32_t flags);

void caninit()
{

    CAND1.rxfull_cb = rxCB;
    canStart(&CAND1, &cancfg);
}

} // namespace CanBusHandler