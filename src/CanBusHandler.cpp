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
static const CANConfig cancfg =
    {CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_TXFP,
     CAN_BTR_SJW(0) | CAN_BTR_TS2(1) | CAN_BTR_TS1(8) | CAN_BTR_BRP(2)};

/*
 * Receiver thread.
 */
static THD_WORKING_AREA(can_rx_wa, 256);
static THD_FUNCTION(can_rx, p)
{
    (void)p;

    event_listener_t el;
    CANRxFrame rxmsg;
    chRegSetThreadName("canRxThd");
    chEvtRegister(&CAND1.rxfull_event, &el, 0);
    while (true)
    {
        if (chEvtWaitAny(ALL_EVENTS))
            while (canReceive(&CAND1, CAN_ANY_MAILBOX, &rxmsg, TIME_IMMEDIATE) == MSG_OK)
            {
                if (rxmsg.SID == 0x201 && rxmsg.DLC == 8)
                {
                    //received from motor 1
                    orientation_1 = rxmsg.data8[0] << 8 | rxmsg.data8[1];
                    rpm_1 = rxmsg.data8[2] << 8 | rxmsg.data8[3];
                    tourqe_1 = rxmsg.data8[4] << 8 | rxmsg.data8[5];
                    receiveCount++;
                }
            }
    }
    chEvtUnregister(&CAND1.rxfull_event, &el);
}

/*
 * Transmitter thread.
 */
static THD_WORKING_AREA(can_tx_wa, 256);
static THD_FUNCTION(can_tx, p)
{
    (void)p;
    chRegSetThreadName("canTxThd");

    CANTxFrame txmsg;
    txmsg.IDE = CAN_IDE_STD;
    txmsg.SID = 0x200;
    txmsg.RTR = CAN_RTR_DATA;
    txmsg.DLC = 8;
    txmsg.data32[0] = 0x55AA55AA;
    txmsg.data32[1] = 0x00FF00FF;

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

void start()
{
    canStart(&CAND1, &cancfg);
    chThdCreateStatic(&can_tx_wa,
                      sizeof(can_tx_wa),
                      NORMALPRIO + 7,
                      can_tx,
                      NULL);
    chThdCreateStatic(&can_rx_wa,
                      sizeof(can_rx_wa),
                      NORMALPRIO + 7,
                      can_rx,
                      NULL);
}

} // namespace CanBusHandler