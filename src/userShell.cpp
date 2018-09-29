/**
 * @brief 
 * 
 * @file shell.cpp
 * @author Alex Au
 * @date 2018-09-20
 */

#include "userShell.hpp"
#include "CanBusHandler.hpp"
#include "main.hpp"
#include <stdio.h>
#include <stdlib.h>

namespace UserShell
{

//forward declaration of shell functions
void shellTestPrint(BaseSequentialStream *chp, int argc, char *argv[]);
void setCurrent(BaseSequentialStream *chp, int argc, char *argv[]);
void toggleLog(BaseSequentialStream *chp, int argc, char *argv[]);

const ShellCommand commands[] =
    {
        {"testPrint", shellTestPrint},
        {"sc", setCurrent},
        {"tl", toggleLog},
        {NULL, NULL}};

static const SerialConfig shellSDConfig =
    {SHELL_BAUD, 0, USART_CR2_STOP1_BITS, 0};

const ShellConfig shell_cfg1 = {
    (BaseSequentialStream *)&SHELL_SD,
    commands};

THD_WORKING_AREA(shell_wa, 512);

void initShell()
{
    sdStart(&SHELL_SD, &shellSDConfig);
    shellInit();
    thread_t *a_thread_pointer =
        chThdCreateStatic(&shell_wa,
                          sizeof(shell_wa),
                          NORMALPRIO + 1,
                          shellThread,
                          (void *)&shell_cfg1);

    chRegSetThreadNameX(a_thread_pointer,
                        "shell");
}

void shellTestPrint(BaseSequentialStream *chp, int argc, char *argv[])
{
    chprintf(chp, "argc = %d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        chprintf(chp, "argv[%d] = %s\n", i, argv[i]);
    }
}

void setCurrent(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)chp;
    if (argc == 1)
    {
        CanBusHandler::setCurrent_1 = strtol(argv[0], NULL, 0);
        chprintf(chp, "set current to %ld\n", CanBusHandler::setCurrent_1);
    }
}

void toggleLog(BaseSequentialStream *chp, int argc, char *argv[])
{
    (void)chp;
    (void)argc;
    (void)argv;
    enable_logging = !enable_logging;
};

} // namespace UserShell