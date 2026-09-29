/**
  ******************************************************************************
  * @file    main.h
  * @brief   Header for main.c.
  *          This file contains the common defines of the application.
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

#ifndef MAIN_H
#define MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "example.h"
#include "mx_system.h"  /* target-specific generated code providing system services */
#include "mx_led.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/**
  * @brief  Hard Fault Handler
  *         Redefines the HardFault handler from the startup file.
  *         (infinite loop)
  *
  *         The default handler is redefined here so that:
  *         1. The example status can be updated.
  *         2. You can easily set a breakpoint to investigate the issue.
  */
void HardFault_Handler(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAIN_H */
