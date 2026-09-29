/**
  ******************************************************************************
  * @file    example.c
  * @brief   example program body
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
/* Includes ------------------------------------------------------------------*/
#include "example.h"
#include "ble_manager.h"

#include "ble_led.h"
#include "ble_environmental.h"
#include "ble_sensor_fusion.h"

#define PRESSURE_MIDDLE_VALUE 1000.0

/* Private macro ------------------------------------------------------------*/

/* Private defines -----------------------------------------------------------*/

/* Imported Variables --------------------------------------------------------*/

/* USER CODE BEGIN IV */

/* USER CODE END IV */

/* Exported Variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */

/* USER CODE END EV */

/* Private Variables ---------------------------------------------------------*/
static uint8_t BlinkLed                         = 0;
static uint8_t RandomEnvEnabled                 = 0;
static uint8_t RandomSensorFusionEnabled        = 0;
static uint8_t LedEnabled                       = 0;
static uint8_t LedStatus                        = 0;

/* Private function prototypes -----------------------------------------------*/
static void user_init(void);
static void user_process(void);
static void compute_random_quaternions(void);
static void button_pressed(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger);

/**
  * @brief  User application initialization.
  * @retval status (see app_status_t)
  */
app_status_t app_init(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Initialize user process */
  user_init();

  /* Initialize all services */
  services_init();

  return_status = EXEC_STATUS_INIT_OK;

  /* _app_init_exit: */
  return return_status;
}

/**
  * @brief  User application processing.
  *         Gets the values of the temperature in Celsius and of the pressure in hPa.
  *         The values are displayed on the terminal.
  * @retval status (see app_status_t)
  *         EXEC_STATUS_OK if OK, EXEC_STATUS_ERROR in case of error
  */
app_status_t app_process(void)
{
  app_status_t return_status = EXEC_STATUS_ERROR;

  /* Process application */
  user_process();

  return_status = EXEC_STATUS_OK;

  /* _app_process_exit: */
  return return_status;
}

/**
  * @brief  User application de-init.
  * @retval status (see app_status_t)
  */
app_status_t app_deinit(void)
{
  return EXEC_STATUS_OK;
}

/**
  * @brief  Initialize User process.
  */
static void user_init(void)
{
  SENSOR_DT_PRINTF("\033[2J\033[1;1f");
  SENSOR_DT_PRINTF("UART Initialized\r\n");

  SENSOR_DT_PRINTF("\r\nSTMicroelectronics %s:\r\n"
                   "\tVersion %c.%c.%c\r\n"
                   "\t%s Board"
                   "\r\n",
                   FW_PACKAGENAME,
                   FW_VERSION_MAJOR, FW_VERSION_MINOR, FW_VERSION_PATCH, STM32_BOARD);

  SENSOR_DT_PRINTF("\n\t(HAL %u.%u.%u_%u)\r\n"
                   "\tCompiled %s %s"
#if defined (__IAR_SYSTEMS_ICC__)
                   " (IAR)\r\n"
#elif defined (__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) /* For ARM Compiler 5 and 6 */
                   " (KEIL)\r\n"
#elif defined (__GNUC__)
                   " (STM32CubeIDE)\r\n"
#endif /* IDE */
                   ,
                   (unsigned int)(HAL_GetVersion() >> 24),
                   (unsigned int)((HAL_GetVersion() >> 16) & 0xFF),
                   (unsigned int)((HAL_GetVersion() >> 8) & 0xFF),
                   (unsigned int)(HAL_GetVersion()      & 0xFF),
                   __DATE__, __TIME__);

  SENSOR_DT_PRINTF("\r\n");

#ifdef SENSOR_DT_DEBUG_CONNECTION
  SENSOR_DT_PRINTF("Debug Connection         Enabled\r\n");
#endif /* SENSOR_DT_DEBUG_CONNECTION */

#ifdef SENSOR_DT_NOTIFY_TRAMISSION
  SENSOR_DT_PRINTF("Debug Notify Transmission Enabled\r\n\n");
#endif /* SENSOR_DT_NOTIFY_TRAMISSION */

  /* Initialize Register Callback for user button */
  hal_exti_handle_t *pEXTI = BUTTON_0_EXTI_GETHANDLE();
  HAL_EXTI_RegisterTriggerCallback(pEXTI, &button_pressed);
  HAL_EXTI_Enable(pEXTI, HAL_EXTI_MODE_INTERRUPT);
}

/**
  * @brief  Button Pressed function.
  * @param  *hexti Pin interrupt handle
  * @param  trigger
  */
void button_pressed(hal_exti_handle_t *hexti, hal_exti_trigger_t trigger)
{
  SENSOR_DT_PRINTF("Button Pressed\r\n\n");
}

/**
  * @brief  Configure the device as Client or Server and manage the communication
  *         between a client and a server.
  */
static void user_process(void)
{
  if (set_connectable)
  {
    set_connectable = 0;
    enable_extended_configuration_commad();
    set_connectable_ble();
    BlinkLed = 1;
  }

  /* handle service event */
  hci_user_evt_proc();

  /* Blinking the Led */
  if (BlinkLed)
  {
    if (LedStatus)
    {
      led_off(MX_STATUS_LED);
    }
    else
    {
      led_on(MX_STATUS_LED);
    }

    LedStatus = !LedStatus;

    /* wait 1 sec (100 + 900) before Led Status changes */
    HAL_Delay(900);
  }

  /* Environmental Data */
  if (RandomEnvEnabled)
  {
    int32_t press_to_send;
    uint16_t hum_to_send;
    int16_t temp_to_send;

    /* Read all the Environmental Sensors */
    set_random_environmental_values(&press_to_send, &hum_to_send, &temp_to_send);

    /* Send environmental data to service event */
    ble_environmental_update(press_to_send, hum_to_send, temp_to_send, 0);

    /* wait 1 sec (100 + 900) before Led Status changes */
    HAL_Delay(900);
  }

  /* MotionFX */
  if (RandomSensorFusionEnabled)
  {
    compute_random_quaternions();

    /* wait 100 ms before sending new data */
    /* HAL_Delay(100); */
  }

  /* Wait next event */
  __WFI();
}

/**
  * @brief  Compute Random Quaternions
  */
static void compute_random_quaternions(void)
{
  static ble_motion_sensor_axes_t quat_axes_send[1] = {{0, 0, 0}};

  static uint32_t counter = 0;

  /* Update Acceleration, Gyroscope and Sensor Fusion data */
  if (counter < 25)
  {
    quat_axes_send[0].axis_x -= (100  + ((uint64_t)rand() * 3 * counter) / RAND_MAX);
    quat_axes_send[0].axis_y += (100  + ((uint64_t)rand() * 5 * counter) / RAND_MAX);
    quat_axes_send[0].axis_z -= (100  + ((uint64_t)rand() * 7 * counter) / RAND_MAX);
  }
  else
  {
    quat_axes_send[0].axis_x += (200 + ((uint64_t)rand() * 7 * counter) / RAND_MAX);
    quat_axes_send[0].axis_y -= (150 + ((uint64_t)rand() * 3 * counter) / RAND_MAX);
    quat_axes_send[0].axis_z += (10  + ((uint64_t)rand() * 5 * counter) / RAND_MAX);
  }

  ble_sensor_fusion_update(quat_axes_send, 1);

  counter ++;
  if (counter == 50)
  {
    counter = 0;
    quat_axes_send[0].axis_x = -quat_axes_send[0].axis_x;
    quat_axes_send[0].axis_y = -quat_axes_send[0].axis_y;
    quat_axes_send[0].axis_z = -quat_axes_send[0].axis_z;
  }
}

/**
  * @brief  Random Environmental Data (Temperature/Pressure/Humidity).
  * @param  *press_to_send pointer to Press Value
  * @param  *hum_to_send  pointer to Humidity Value
  * @param  *temp_to_send pointer to Temperature Value
  */
void set_random_environmental_values(int32_t *press_to_send, uint16_t *hum_to_send, int16_t *temp_to_send)
{
  float random_value;
  int32_t dec_part, int_part;

  *press_to_send = 0;
  *hum_to_send = 0;
  *temp_to_send = 0;

  /* P sensor emulation */
  random_value = PRESSURE_MIDDLE_VALUE + ((uint64_t)rand() * 80) / RAND_MAX;
  MCR_BLUEMS_F2I_2D(random_value, int_part, dec_part);
  *press_to_send = int_part * 100 + dec_part;

  /* H sensor emulation */
  random_value = 60.0 + ((uint64_t)rand() * 20) / RAND_MAX;
  MCR_BLUEMS_F2I_1D(random_value, int_part, dec_part);
  *hum_to_send = int_part * 10 + dec_part;

  /* T sensor emulation */
  random_value = 27.0 + ((uint64_t)rand() * 5) / RAND_MAX;
  MCR_BLUEMS_F2I_1D(random_value, int_part, dec_part);
  *temp_to_send = int_part * 10 + dec_part;
}

/**
  * @brief  This function is called when there is a LE Connection Complete event.
  * @param  connection_handle
  * @param  address_type
  * @param  addr[]
  */
void connection_completed_function(uint16_t connection_handle, uint8_t address_type, uint8_t addr[6])
{
  BlinkLed = 0;

  /* Led green off */
  led_off(MX_STATUS_LED);

  LedStatus = 0;

  SENSOR_DT_PRINTF("Call to connection_completed_function\r\n");
  HAL_Delay(100);
}

/**
  * @brief  This function is called when the peer device get disconnected.
  */
void disconnection_completed_function(void)
{
  LedEnabled = 0;
  RandomEnvEnabled = 0;
  RandomSensorFusionEnabled = 0;

  SENSOR_DT_PRINTF("Call to disconnection_completed_function\r\n");
  HAL_Delay(100);
}

/**
  * @brief  This function makes the parsing of the Debug Console.
  * @param  *att_data attribute data
  * @param  data_length length of the data
  * @retval send_back_data true/false
  */
uint32_t debug_console_parsing(uint8_t *att_data, uint8_t data_length)
{
  uint32_t send_back_data = 1;

  /* Help Command */
  if (!strncmp("help", (char *)(att_data), 4))
  {
    /* Print Legend */
    send_back_data = 0;

    bytes_to_write = sprintf((char *)buffer_to_write, "Command:\r\n"
                             "info-> System Info\r\n"
                             "uid-> STM32 UID value\r\n");
    term_update(buffer_to_write, bytes_to_write);
  }
  else if (!strncmp("info", (char *)(att_data), 4))
  {
    send_back_data = 0;

    bytes_to_write = sprintf((char *)buffer_to_write, "\r\nSTMicroelectronics %s:\r\n"
                             "\tVersion %c.%c.%c\r\n"
                             "\t%s board"
                             "\r\n",
                             FW_PACKAGENAME,
                             FW_VERSION_MAJOR, FW_VERSION_MINOR, FW_VERSION_PATCH, STM32_BOARD);

    term_update(buffer_to_write, bytes_to_write);

    bytes_to_write = sprintf((char *)buffer_to_write, "\t(HAL %u.%u.%u_%u)\r\n"
                             "\tCompiled %s %s"
#if defined (__IAR_SYSTEMS_ICC__)
                             " (IAR)\r\n",
#elif defined (__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) /* For ARM Compiler 5 and 6 */
                             " (KEIL)\r\n",
#elif defined (__GNUC__)
                             " (STM32CubeIDE)\r\n",
#endif /* IDE */
                             (unsigned int)(HAL_GetVersion() >> 24),
                             (unsigned int)((HAL_GetVersion() >> 16) & 0xFF),
                             (unsigned int)((HAL_GetVersion() >> 8) & 0xFF),
                             (unsigned int)(HAL_GetVersion()      & 0xFF),
                             __DATE__, __TIME__);

    term_update(buffer_to_write, bytes_to_write);
  }
  else if ((att_data[0] == 'u') & (att_data[1] == 'i') & (att_data[2] == 'd'))
  {
    /* Write back the STM32 UID */
    uint8_t *uid = (uint8_t *)BLE_STM32_UUID;
    uint32_t MCU_ID = BLE_STM32_MCU_ID[0] & 0xFFF;
    bytes_to_write = sprintf((char *)buffer_to_write, "%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X%.2X_%.3lX\r\n",
                             uid[ 3], uid[ 2], uid[ 1], uid[ 0],
                             uid[ 7], uid[ 6], uid[ 5], uid[ 4],
                             uid[11], uid[ 10], uid[9], uid[8],
                             (unsigned long)MCU_ID);
    term_update(buffer_to_write, bytes_to_write);
    send_back_data = 0;
  }

  return send_back_data;
}

/**
  * @brief  Callback Function for Config write request.
  * @param *att_data attribute data
  * @param data_length length of the data
  */
void write_request_config_function(uint8_t *att_data, uint8_t data_length)
{
  uint32_t feature_mask = (att_data[3]) | (att_data[2] << 8) | (att_data[1] << 16) | (att_data[0] << 24);
  uint8_t command = att_data[4];
  uint8_t data    = att_data[5];

  switch (feature_mask)
  {
    case FEATURE_MASK_LED:
      /* Led events */
#ifdef SENSOR_DT_DEBUG_CONNECTION
      if (ble_std_term_service == BLE_SERV_ENABLE)
      {
        bytes_to_write = sprintf((char *)buffer_to_write,
                                 "Conf Sig F=%lx C=%2x\n\r",
                                 (unsigned long)feature_mask, command);
        term_update(buffer_to_write, bytes_to_write);
      }
      else
      {
        SENSOR_DT_PRINTF("Conf Sig F=%lx C=%2x\r\n", (unsigned long)feature_mask, command);
      }
#endif /* SENSOR_DT_DEBUG_CONNECTION */
      switch (command)
      {
        case 1:
          /* Led green on */
          led_on(MX_STATUS_LED);
          LedStatus = 1;
          config_update(FEATURE_MASK_LED, command, data);
          break;
        case 0:
          /* Led green off */
          led_off(MX_STATUS_LED);
          LedStatus = 0;
          config_update(FEATURE_MASK_LED, command, data);
          break;
      }
      /* Update the LED feature */
      if (LedEnabled)
      {
        ble_led_status_update(LedStatus);
      }
      break;
  }
}

/***************************************************
  * Callback functions to manage the notify events *
  **************************************************/

/**
  * @brief  Callback Function for Un/Subscription Feature.
  * @param  event Sub/Unsub
  */
void notify_event_env(ble_notify_event_t event)
{
  /* Environmental Features */
  if (event == BLE_NOTIFY_SUB)
  {
    RandomEnvEnabled = 1;
  }

  if (event == BLE_NOTIFY_UNSUB)
  {
    RandomEnvEnabled = 0;
  }
}

/**
  * @brief  Callback Function for Un/Subscription Feature.
  * @param  event Sub/Unsub
  */
void notify_event_led(ble_notify_event_t event)
{
  /* Led Features */
  if (event == BLE_NOTIFY_SUB)
  {
    LedEnabled = 1;
    ble_led_status_update(LedStatus);
  }

  if (event == BLE_NOTIFY_UNSUB)
  {
    LedEnabled = 0;
  }
}

/**
  * @brief  Callback Function for Led read request.
  * @param  *led_status Status of the led
  */
void read_request_led_function(uint8_t *led_status)
{
  *led_status = LedStatus;
}

/**
  * @brief  Callback Function for Un/Subscription Feature.
  * @param  event Sub/Unsub
  */
void notify_event_sensor_fusion(ble_notify_event_t event)
{
  /* Sensor Fusion Features */
  if (event == BLE_NOTIFY_SUB)
  {
    RandomSensorFusionEnabled = 1;
  }

  if (event == BLE_NOTIFY_UNSUB)
  {
    RandomSensorFusionEnabled = 0;
  }
}

/**
  * @brief  Set Board Name.
  */
void set_board_name(void)
{
  sprintf(ble_stack_value.board_name, "%s%c%c%c", "SDTR",
          FW_VERSION_MAJOR,
          FW_VERSION_MINOR,
          FW_VERSION_PATCH);
}

/**
  * @brief  Callback Function for answering to Info command.
  * @param  *answer Return String
  */
void ext_config_info_command_callback(uint8_t *answer)
{
  sprintf((char *)answer, "STMicroelectronics %s:\n"
          "Version %c.%c.%c\n"
          "%s board\n"
          "(HAL %u.%u.%u_%u)\n"
          "Compiled %s %s"
#if defined (__IAR_SYSTEMS_ICC__)
          " (IAR)",
#elif defined (__CC_ARM) || (defined(__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)) /* For ARM Compiler 5 and 6 */
          " (KEIL)",
#elif defined (__GNUC__)
          " (STM32CubeIDE)",
#endif /* IDE */
          FW_PACKAGENAME,
          FW_VERSION_MAJOR,
          FW_VERSION_MINOR,
          FW_VERSION_PATCH,
          STM32_BOARD,
          (unsigned int)(HAL_GetVersion() >> 24),
          (unsigned int)((HAL_GetVersion() >> 16) & 0xFF),
          (unsigned int)((HAL_GetVersion() >> 8) & 0xFF),
          (unsigned int)(HAL_GetVersion()      & 0xFF),
          __DATE__, __TIME__);
}

/**
  * @brief  Callback Function for answering to VersionFw command.
  * @param  *answer Return String
  */
void ext_config_version_fw_command_callback(uint8_t *answer)
{
  sprintf((char *)answer, "%s_%c.%c.%c",
          FW_PACKAGENAME,
          FW_VERSION_MAJOR,
          FW_VERSION_MINOR,
          FW_VERSION_PATCH);

  BLE_MANAGER_PRINTF("Call to ext_config_version_fw_command_callback (It is a weak function)\r\n");
}
