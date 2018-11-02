#include <string.h>

#include "ch.h"
#include "hal.h"

#include <stdio.h>
#include <string.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"
#include "stm32f1xx.h"
#include "flash.hpp"

#define FlASH_START_ADDRESS (0x08000000U + (FLASH_TOTAL_PAGE_COUNT - FLASH_STORAGE_PAGE_COUNT) * FLASH_PAGE_SIZE)

namespace flashStorage
{

inline void wait()
{
	while (FLASH->SR & FLASH_SR_BSY)
		chThdSleep(1);
}

inline void lock()
{
	FLASH->CR |= FLASH_CR_LOCK;
}

static void unlock(void)
{
	//check if locked
	while (FLASH->CR & FLASH_CR_LOCK)
	{
		//unlock sequence
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
		//check unlocked
		if (!(FLASH->CR & FLASH_CR_LOCK))
			return;
	}
}

staticContent_t content = {};

bool store()
{
	unlock();

	wait();
	//start page erase
	FLASH->CR |= FLASH_CR_PER;

	int pageCount = (sizeof(content) / FLASH_PAGE_SIZE) + 1;

	static uint16_t *flashPtr = (uint16_t *)FlASH_START_ADDRESS;
	static int i = 0;
	for (i = 0; i < pageCount; i++)
	{
		FLASH->AR = (uint32_t)flashPtr;
		FLASH->CR |= FLASH_CR_STRT;
		wait();
		flashPtr += FLASH_PAGE_SIZE;
	}

	FLASH->CR &= ~FLASH_CR_PER;

	//start programming, half word
	FLASH->CR |= FLASH_CR_PG;

	flashPtr = (uint16_t *)FlASH_START_ADDRESS;
	uint16_t *dataPtr = (uint16_t *)&content;
	//calculate how many 16bit write is required
	int dataCount = sizeof(content) / 2;
	for (int i = 0; i < dataCount; i++)
	{
		*flashPtr = *dataPtr;
		wait();
		dataPtr++;
		flashPtr++;
	}

	//handle the 1 byte not programmed
	if (sizeof(content) % 2)
	{
		uint16_t temp = *(uint8_t *)dataPtr;
		*flashPtr = temp;
		wait();
	}

	FLASH->CR &= ~FLASH_CR_PG;

	lock();

	//verify content
	if (memcmp((void *)FlASH_START_ADDRESS, &content, sizeof(content)))
		return false;
	else
		return true;
};

bool read()
{
	//TODO: verify centent with crc before performing action
	if (*(char *)FlASH_START_ADDRESS != 0xff)
	{
		content = *(staticContent_t *)FlASH_START_ADDRESS;
		return true;
	}
	else
	{
		store();
		return false;
	}
};

} // namespace flashStorage