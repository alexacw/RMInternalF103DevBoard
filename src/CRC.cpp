#include "CRC.hpp"

#define CRC_DR (*(volatile uint32_t *)CRC)
#define CRC_IDR *((volatile uint32_t *)CRC + 1)
#define CRC_CR *((volatile uint32_t *)CRC + 2)

namespace hw_crc32
{

uint32_t calculate(uint8_t *input, size_t size)
{
    int i;
    for (i = 0; size - i >= 4; i += sizeof(uint32_t))
    {
        CRC->DR = *(uint32_t *)(input + i);
        CRC_DR;
    }
    int remainBytes;
    if ((remainBytes = size % sizeof(uint32_t)))
    {
        uint32_t temp;
        memcpy(&temp, input + i, remainBytes);
        CRC_DR = temp;
    }
    return CRC_DR;
}

void reset()
{
    CRC_CR |= CRC_CR_RESET;
}

void init()
{
    RCC->AHBENR |= RCC_AHBENR_CRCEN_Msk;
    (void)RCC->APB1ENR;
}

} // namespace hw_crc32
