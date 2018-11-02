#include "ch.h"
#include "hal.h"

#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "chprintf.h"
#include "stm32f1xx.h"

//these must be unsigned
#define FLASH_TOTAL_PAGE_COUNT 64U  //all pages the mcu have
#define FLASH_STORAGE_PAGE_COUNT 8U //pages used for storing persistence data
#define FLASH_PAGE_SIZE 1024U

namespace flashStorage
{

enum Dtype : uint8_t
{
    dt_float,
    dt_double,
    dt_uint32,
    dt_int32
};

struct Record_t
{
    Dtype type;
};

//struct defining the storage structure
struct staticContent_t
{
    char testString[11];
    Record_t sdsd;
};

extern staticContent_t content;

/**
 * @brief 
 * store all content to the flash, return sucess status
 */
bool store();

/**
 * @brief read all data from flash to content, return sucess status
 */
bool read();
} // namespace flashStorage
