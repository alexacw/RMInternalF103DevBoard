#include "ch.h"
#include "hal.h"

#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "chprintf.h"
#include "stm32f1xx.h"

//these must be unsigned
#define FLASH_TOTAL_PAGE_COUNT 64U
#define FLASH_STORAGE_PAGE_COUNT 8U
#define FLASH_PAGE_SIZE 1024U

/**
 * @brief number of pages reserved at the end of flash memory for storing data
 * 
 */

namespace flashStorage
{

//struct defining the storage structure
struct flashStorageContent_t
{
    char testString[11];
};

extern flashStorageContent_t content;

bool writeFlashAll();
bool readFlashAll();
} // namespace flashStorage
