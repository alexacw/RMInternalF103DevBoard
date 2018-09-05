/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#include "ch.hpp"

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"

#include "MPU6050.h"

using namespace chibios_rt;

/*===========================================================================*/
/* Command line related.                                                     */
/*===========================================================================*/

#define SHELL_WA_SIZE THD_WORKING_AREA_SIZE(2048)

/* Can be measured using dd if=/dev/xxxx of=/dev/null bs=512 count=10000.*/
static void cmd_write(BaseSequentialStream *chp, int argc, char *argv[])
{
  static uint8_t buf[] =
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"
      "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";

  (void)argv;
  if (argc > 0)
  {
    chprintf(chp, "Usage: write\r\n");
    return;
  }

  while (chnGetTimeout((BaseChannel *)chp, TIME_IMMEDIATE) == Q_TIMEOUT)
  {
#if 1
    /* Writing in channel mode.*/
    chnWrite(&SD1, buf, sizeof buf - 1);
#else
    /* Writing in buffer mode.*/
    (void)obqGetEmptyBufferTimeout(&SDU1.obqueue, TIME_INFINITE);
    memcpy(SDU1.obqueue.ptr, buf, SERIAL_USB_BUFFERS_SIZE);
    obqPostFullBuffer(&SDU1.obqueue, SERIAL_USB_BUFFERS_SIZE);
#endif
  }
  chprintf(chp, "\r\n\nstopped\r\n");
}

static const ShellCommand commands[] = {
    {"write", cmd_write},
    {NULL, NULL}};

static const ShellConfig shell_cfg1 = {
    (BaseSequentialStream *)&SD1,
    commands};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * Blinker thread, times are in milliseconds.
 * C++ version
 */
class BlinkerThread : public BaseStaticThread<128>
{
protected:
  void main(void) override
  {
    setName("blinker");

    while (true)
    {
      palClearLine(LINE_LED);
      chThdSleepMilliseconds(500);
      palSetLine(LINE_LED);
      chThdSleepMilliseconds(500);
    }
  }

public:
  BlinkerThread(void) : BaseStaticThread<128>()
  {
  }
};

/* C version :

static THD_WORKING_AREA(waThread1, 128);
static __attribute__((noreturn)) THD_FUNCTION(Thread1, arg)
{

  (void)arg;
  chRegSetThreadName("blinker");
  while (true)
  {
    palClearLine(LINE_LED);
    chThdSleepMilliseconds(500);
    palSetLine(LINE_LED);
    chThdSleepMilliseconds(500);
  }

} */

/* do not create thread objects inside functions */
BlinkerThread blinker;

static const I2CConfig i2c2cfg = {
    OPMODE_I2C,
    400000,
    FAST_DUTY_CYCLE_2,
};

/*
 * Application entry point.
 */
int main(void)
{

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  sdStart(&SD1, NULL);

  i2cStart(&I2CD2, &i2c2cfg);

  /*
   * Shell manager initialization.
   */
  shellInit();

  /*
   * Creates the blinker thread, C method.
   */
  // chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Creates the blinker thread, C++ method.
   */
  blinker.start(NORMALPRIO);

  //chThdSleepMilliseconds(500);
  sdWrite(&SD1, (const uint8_t *)"\n\n\n\n", 4);
  MPU6050(0b1101000);
  MPUinitialize();
  if (MPUtestConnection())
    chprintf((BaseSequentialStream *)&SD1, "MPU6050 connection ok\n");
  else
    chprintf((BaseSequentialStream *)&SD1, "MPU6050 connection failed\n");

  chThdSleepMilliseconds(500);

  //start the shell over UART1
  thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE,
                                          "shell", NORMALPRIO + 1,
                                          shellThread, (void *)&shell_cfg1);

  /*
   * Normal main() thread activity, spawning shells.
   */
  while (true)
  {
    chprintf((BaseSequentialStream *)&SD1,
             "ax: %d ay:%d az:%d\n",
             MPUgetAccelerationX(),
             MPUgetAccelerationY(),
             MPUgetAccelerationZ());

    chThdSleepMilliseconds(100);
  }
}
