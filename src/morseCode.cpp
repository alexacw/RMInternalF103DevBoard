#include "morseCode.hpp"

using namespace chibios_rt;

namespace MorseCode
{

const char str[] = "champion";

event_source_t userIntperrupt;

//morse code from a to z
const char *code[] =
    {".-", "-...", "-.-.", "-..", ".",     //abcde
     "..-.", "--.", "....", "..", ".---",  //fghij
     "-.-", ".-..", "--", "-.", "---",     //klmno
     ".--.", "--.-", ".-.", "...", "-",    //pqrst
     "..-", "...-", ".--", "-..-", "-.--", //uvwxy
     "--.."};                              //z

//just showcasing how threads can be created in a oop way
class MorseCodeThd : public BaseStaticThread<64>
{
  public:
    MorseCodeThd(const char *const c_str)
        : BaseStaticThread<64>(),
          c_str(c_str) {}

  protected:
    void main() override
    {
        bool was_interrupted = false;
        const char *current_char_ptr = c_str;
        chEvtRegister(&Button::ButtonPressedEvt, &buttonLis, 0);
        chEvtRegister(&userIntperrupt, &userLis, 1);

        while (!this->shouldTerminate())
        {
            palWriteLine(LINE_LED, ~LED_ON_STATE);
            chThdSleepMilliseconds(200);

            if (!was_interrupted)
                //wait for button press, break if user request to turn off
                if (chEvtWaitAny(ALL_EVENTS) == EVENT_MASK(1))
                    break;

            was_interrupted = false;

            if (*current_char_ptr == '\0') //check whole string complete
                current_char_ptr = c_str;

            // find character index, a = 0, z = 25
            unsigned int index = 0;
            if (*current_char_ptr >= 'a' && *current_char_ptr <= 'z')
                index = *current_char_ptr - 'a';
            else if (*current_char_ptr >= 'A' && *current_char_ptr <= 'Z')
                index = *current_char_ptr - 'A';
            else
            {
                current_char_ptr++;
                continue; //wait for next button press if character is not alphabet
            }

            for (unsigned int subindex = 0;
                 code[index][subindex] != '\0';
                 subindex++)
            {
                unsigned int delayms;
                switch (code[index][subindex])
                {
                case '-':
                    delayms = 750;
                    break;
                case '.':
                    delayms = 250;
                    break;
                default:
                    continue; // not dash or dot?? probably wont happen anyway
                }
                palWriteLine(LINE_LED, LED_ON_STATE);
                if ((was_interrupted = chEvtWaitAnyTimeout(ALL_EVENTS, TIME_MS2I(delayms))))
                    break;
                palWriteLine(LINE_LED, ~LED_ON_STATE);
                if ((was_interrupted = chEvtWaitAnyTimeout(ALL_EVENTS, TIME_MS2I(250))))
                    break;
            }

            current_char_ptr++;
        }

        chEvtUnregister(&Button::ButtonPressedEvt, &buttonLis);
        chEvtUnregister(&userIntperrupt, &userLis);
    }

  private:
    const char *c_str;
    event_listener_t buttonLis;
    event_listener_t userLis;
};

static MorseCodeThd thd(str);

chibios_rt::ThreadReference thdRef = NULL;

void init()
{
    chEvtObjectInit(&userIntperrupt);
};

void start()
{
    palSetLineMode(LINE_LED, PAL_MODE_OUTPUT_OPENDRAIN);
    if (thdRef.isNull())
    {
        thdRef = thd.start(NORMALPRIO);
    }
};

void stop()
{
    if (!thdRef.isNull())
    {
        thdRef.requestTerminate();
        chEvtBroadcast(&userIntperrupt);
        thdRef.wait();
    }
};

} // namespace MorseCode
