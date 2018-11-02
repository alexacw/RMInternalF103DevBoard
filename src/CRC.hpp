#include "ch.h"
#include "ch.hpp"
#include "hal.h"
#include <cstdint>
#include <cstring>

namespace hw_crc32
{

uint32_t calculate(uint8_t *input, size_t size);

void reset();
void init();

} // namespace hw_crc32