#ifndef HAL_SPI_H
#define HAL_SPI_H

#include "bmlite_hal.h"
#include "fpc_bep_types.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Function prototype
fpc_bep_result_t hal_bmlite_spi_write_read(uint8_t *write, uint8_t *read, size_t size, bool leave_cs_asserted);

#endif // HAL_SPI_H
