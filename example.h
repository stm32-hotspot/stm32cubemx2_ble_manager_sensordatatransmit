/**
  ******************************************************************************
  * @file    example.h
  * @brief   Header for example.c.
  *          This file contains example-specific declarations to interface with main().
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef EXAMPLE_H
#define EXAMPLE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

#include "mx_hal_def.h"
#include "mx_led.h"
#include "mx_button.h"
#include "sensor_data_transmit_config.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Exported types ------------------------------------------------------------*/
/* The volatile qualifier ensures the content of variables of this type is always visible in the debugger. */
typedef volatile enum
{
  EXEC_STATUS_ERROR   = -1, /* problem encountered         */
  EXEC_STATUS_UNKNOWN = 0,  /* default value               */
  EXEC_STATUS_INIT_OK = 1,  /* app_init ran as expected    */
  EXEC_STATUS_OK      = 2   /* application ran as expected */
} app_status_t;

/* Exported macro ------------------------------------------------------------*/
#define MCR_BLUEMS_F2I_1D(in, out_int, out_dec) {out_int = (int32_t)in; out_dec= (int32_t)((in-out_int)*10);};
#define MCR_BLUEMS_F2I_2D(in, out_int, out_dec) {out_int = (int32_t)in; out_dec= (int32_t)((in-out_int)*100);};

/* Exported constants --------------------------------------------------------*/
/* Exported macros -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/**
  * @brief  User application initialization.
  * @retval status (see app_status_t)
  */
app_status_t app_init(void);

/**
  * @brief  User application processing.
  *         Gets the values of the temperature in Celsius and of the pressure in hPa.
  *         The values are displayed on the terminal.
  * @retval status (see app_status_t)
  *         EXEC_STATUS_OK if OK, EXEC_STATUS_ERROR in case of error
  */
app_status_t app_process(void);

/**
  * @brief  User application de-init.
  * @retval status (see app_status_t)
  */
app_status_t app_deinit(void);

extern void set_random_environmental_values(int32_t *press_to_send, uint16_t *hum_to_send, int16_t *temp_to_send);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* EXAMPLE_H */
