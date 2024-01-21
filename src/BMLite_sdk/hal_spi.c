/*
 * Copyright (c) 2020 Fingerprint Cards AB
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Modified by Andrey Perminov <andrey.ppp@gmail.com> 
 * for FPC BM-Lite applications
 */

/**
 * @file    hal_spi.c
 * @brief   SPI HAL functions
 */
#define BMLITE_ON_SPI 
#ifdef BMLITE_ON_SPI


#include "bmlite_hal.h"
#include "fpc_bep_types.h"
#include "hal_spi.h"



static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */
static uint8_t       *m_tx_buf;
static uint8_t       *m_rx_buf;

static void spi_write_read(uint8_t *write, uint8_t *read, size_t size, bool leave_cs_asserted)
{


	


	spi_xfer_done = false;
	m_tx_buf =  write;
	m_rx_buf = read;

    // gpio_pin_set(dev, CS_N, 0);   // clear CS pin


	spi_read_write(&write, &read, size);


	// while (!spi_xfer_done)
	// {
	// 	__WFE(); 				//Wait For Event
	// }

	// if(!leave_cs_asserted) gpio_pin_set(dev, CS_N, 1);   //set CS pin

}

fpc_bep_result_t hal_bmlite_spi_write_read(uint8_t *write, uint8_t *read, size_t size,bool leave_cs_asserted)
{

	uint8_t num_of_rounds = size/255, i;
	uint8_t *p_write = write, *p_read = read;

	for(i=0; i<num_of_rounds; i++){
		spi_write_read(p_write, p_read, 255, true);
		p_write += 255;
		p_read += 255;
		size -=255;
	}

	spi_write_read(p_write, p_read, size, leave_cs_asserted);

	return FPC_BEP_RESULT_OK;
}



#endif