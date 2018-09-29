#include "CanBusHandler.hpp"
#include "ch.hpp"
#include "hal.h"
#include "chprintf.h"

namespace CanBusHandler
{

volatile int32_t receiveCount = 0;

//control command
volatile int16_t setCurrent_1 = 0;

//motor feedbacks
volatile int16_t tourqe_1 = 0;
volatile int16_t rpm_1 = 0;
volatile int16_t orientation_1 = 0; // [0,8191] -> [0,360 degree)

/**
   * @brief  
   * Internal loopback mode, 1MBaud, automatic wakeup, automatic recover
   * from abort mode.
   * See section 22.7.7 on the STM32 reference manual.
   */
static const CANConfig cancfg = {
    CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
    CAN_BTR_SJW(0) | CAN_BTR_TS2(1) |
        CAN_BTR_TS1(8) | CAN_BTR_BRP(2)};

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
void rxCB(CANDriver *canp, uint32_t flags)
{
    (void)flags;
    static CANRxFrame f;
    chSysLockFromISR();
    //funny, when frame is received false is returned
    while (!canTryReceiveI(canp, CAN_ANY_MAILBOX, &f))
    {
        if (f.SID == 0x201 && f.DLC == 8)
        {
            //received from motor 1
            orientation_1 = f.data8[0] << 8 | f.data8[1];
            rpm_1 = f.data8[2] << 8 | f.data8[3];
            tourqe_1 = f.data8[4] << 8 | f.data8[5];
            receiveCount++;
        }
    }
    chSysUnlockFromISR();
};

class TransmitterThread : public chibios_rt::BaseStaticThread<256>
{
  public:
    TransmitterThread(CANDriver *canp)
        : chibios_rt::BaseStaticThread<256>(),
          canp(canp)
    {
    }

  protected:
    CANDriver *canp;
    void main(void) override
    {
        setName("canTransmitterThread");

        CANTxFrame txmsg;
        txmsg.IDE = CAN_IDE_STD;
        txmsg.SID = 0x200;
        txmsg.RTR = CAN_RTR_DATA;
        txmsg.DLC = 8;
        while (true)
        {
            systime_t t = chVTGetSystemTime();
            txmsg.data8[0] = setCurrent_1 >> 8;
            txmsg.data8[1] = setCurrent_1;
            canTransmitTimeout(&CAND1, CAN_ANY_MAILBOX, &txmsg, TIME_MS2I(100));

            //200Hz
            chThdSleepUntil(t + TIME_MS2I(50));
        }
    }
};

static TransmitterThread tThd = TransmitterThread(&CAND1);

void start(CANDriver *canp)
{
    if (canp)
    {
        canp->rxfull_cb = rxCB;
        canp->error_cb = NULL;
        canp->txempty_cb = NULL;
        canStart(canp, &cancfg);

        //chThdCreateStatic(can_rx_wa, sizeof(can_rx_wa), NORMALPRIO + 7, can_rx, NULL);
        tThd.start(NORMALPRIO + 3);
    }
}

} // namespace CanBusHandler