#include "PWM_Ctrl.hpp"

using namespace chibios_rt;

namespace PWM_Ctrl
{

static PWMConfig pwm1cfg = {
    PWM1_FREQ,   /* 100kHz PWM clock frequency.   */
    PWM1_PERIOD, /* Initial PWM period 10ms       */
    NULL,
    {{PWM_OUTPUT_ACTIVE_HIGH, NULL},
     {PWM_OUTPUT_DISABLED, NULL},
     {PWM_OUTPUT_DISABLED, NULL},
     {PWM_OUTPUT_DISABLED, NULL}},
    0,
    0};

class BreathLightThd : public BaseStaticThread<64>
{
  public:
    BreathLightThd(){};

  protected:
    void main() override
    {
        this->setName("BreathLightThd");
        while (!this->shouldTerminate())
        {
            if (LED_PWM_DRIVER.tim->CCR[LED_PWM_CHANNEL] == 0)
                increasing = true;
            else if (LED_PWM_DRIVER.tim->CCR[LED_PWM_CHANNEL] >= PWM1_PERIOD)
                increasing = false;

            if (increasing)
                LED_PWM_DRIVER.tim->CCR[LED_PWM_CHANNEL] += 3;
            else
                LED_PWM_DRIVER.tim->CCR[LED_PWM_CHANNEL] -= 3;
            this->sleep(TIME_MS2I(1));
        }
    }

  private:
    bool increasing;
};

static BreathLightThd thd;
chibios_rt::ThreadReference thdRef = NULL;

void startBreathLight()
{
    palSetLineMode(LINE_LED, PAL_MODE_STM32_ALTERNATE_OPENDRAIN);
    if (thdRef.isNull())
    {

        pwmStart(&LED_PWM_DRIVER, &pwm1cfg);
        pwmEnableChannel(&LED_PWM_DRIVER, LED_PWM_CHANNEL, 0);
        thdRef = thd.start(NORMALPRIO);
    }
};

void stopBreathLight()
{
    if (!thdRef.isNull())
    {
        thdRef.requestTerminate();
        thdRef.wait();
    }
};

} // namespace PWM_Ctrl
