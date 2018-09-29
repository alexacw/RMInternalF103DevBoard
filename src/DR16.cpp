/**
 * Edward ZHANG, Terry ZENG, 20180304
 * Alex Au reworked for recuitment internal competetion 20180928 
 * @file    DR16.c
 * @brief   Receiver for DJI DR16 Robomaster specific RC
 */

#include "DR16.hpp"
#include "chprintf.h"
namespace DR16
{
volatile RC_Value_t rcValue;
static volatile RC_Msg_t rxbuf;
static volatile systime_t lastReceive;
volatile bool isConnected = false;

static void decodeDR16()
{
	rcValue.rc.ch0 = rxbuf.rc.ch0_l | rxbuf.rc.ch0_h << 8;
	rcValue.rc.ch1 = rxbuf.rc.ch1_l | rxbuf.rc.ch1_h << 5;
	rcValue.rc.ch2 = rxbuf.rc.ch2_l | rxbuf.rc.ch2_m << 2 | rxbuf.rc.ch2_h << 10;
	rcValue.rc.ch3 = rxbuf.rc.ch3_l | rxbuf.rc.ch3_h << 7;
	rcValue.rc.s1 = rxbuf.rc.s1;
	rcValue.rc.s2 = rxbuf.rc.s2;
}

/**
 * UART driver configuration structure.
 */
static UARTConfig uart_cfg = {
	NULL, NULL, NULL, NULL, NULL,
	100000,						 //baud rate
	USART_CR1_M | USART_CR1_PCE, //parity control enable, 8+1 bits
	USART_CR2_LBDL,				 //10 bit break detection
	0};

static THD_WORKING_AREA(DR16_receiver_thread_wa, 256);
static THD_FUNCTION(DR16_receiver_thread, p)
{
	(void)p;
	chRegSetThreadName("DR16 receiver");

	volatile size_t rxBytes;
	while (!chThdShouldTerminateX())
	{
		rxBytes = sizeof(rxbuf);
		if (MSG_OK ==
			uartReceiveTimeout(&DR16_UART_DRIVER,
							   (size_t *)&rxBytes,
							   (void *)&rxbuf,
							   TIME_MS2I(DR16_RECEIVE_TIMEOUT_MS)))
		{
			decodeDR16(); //quite atomic write, can implement mutex if input combination is important
			chThdSleepMilliseconds(DR16_RECEIVE_WAIT_MS);
		}
	}
}

void resetValue()
{
	rcValue.rc.ch0 = RC_CH_VALUE_MID;
	rcValue.rc.ch1 = RC_CH_VALUE_MID;
	rcValue.rc.ch2 = RC_CH_VALUE_MID;
	rcValue.rc.ch3 = RC_CH_VALUE_MID;
	rcValue.mouse.press_l = 0;
	rcValue.mouse.press_r = 0;
	rcValue.key.v = 0;
}

/**
 * @brief   Initialize the RC receiver
 */
void start(void)
{

	uartStart(&DR16_UART_DRIVER, &uart_cfg);

	chThdCreateStatic(DR16_receiver_thread_wa,
					  sizeof(DR16_receiver_thread_wa),
					  NORMALPRIO + 7,
					  DR16_receiver_thread,
					  NULL);
}
} // namespace DR16
