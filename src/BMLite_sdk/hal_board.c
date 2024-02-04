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
 * @file   hal_board.c
 * @brief  Board Specific functions
 */

#include <stdbool.h>

#include "bmlite_hal.h"

// #define BMLITE_PIN_RESET   	ARDUINO_2_PIN
// #define BMLITE_PIN_STATUS   ARDUINO_A2_PIN

static volatile bool sensor_interrupt = false;


fpc_bep_result_t hal_board_init(void *params)
{


    return FPC_BEP_RESULT_OK;
}

void hal_bmlite_reset(bool state)
{
    // if(!state) {
	//     nrf_drv_gpiote_out_set(BMLITE_PIN_RESET);
    // } else {
    //     nrf_drv_gpiote_out_clear(BMLITE_PIN_RESET);
    // }
}


