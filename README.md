<img src="doc/subbrand-stm32.svg" width="50" alt="STM32 Subbrand Logo"/>

# __Example: *SensorDataTransmit*__

**Example version:** 1.0.0

[![User Manual](doc/read_the-UM.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/index.html "An offline version is also available in the STM32Cube firmware package.")

How to use the STM32_BLE_Manager middleware to transmit environmental data and sensor-fusion information over Bluetooth Low Energy and to control an LED from the ST BLE Sensor mobile application.
This example sends temperature, humidity, pressure, and quaternion data to a mobile device.
It also receives a Bluetooth command to switch the board LED on or off.
The transmitted environmental and orientation values are randomly generated to demonstrate BLE connectivity only.

## __1. Detailed scenario__

__Initialization phase__: At main program start, the `mx_system_init()` function is called. It initializes the peripherals, the system clock, and the SysTick.

The application executes the following __example steps__:

__Step 1__: The application initializes the UART interface.

__Step 2__: The application initializes the Bluetooth stack.

__Step 3__: The application initializes and adds the Environmental, LED, and Sensor Fusion BLE features.

__Step 4__: The application enables Bluetooth connectivity in advertising mode.
            Returns to step 4 indefinitely if no error occurs.

__End of example__: This is an endless example that remains in advertising or connected mode and handles BLE data exchange continuously.

You can verify that the example runs properly via the terminal logs.

If you enable `USE_TRACE`, you can follow these execution steps in the terminal logs:

```text
[INFO] Step 1: UART Initialized
[INFO] Step 1: STMicroelectronics SensorDataTransmit:
[INFO] Step 1:         Version 1.0.0
[INFO] Step 1:         NUCLEO-C562RE Board
[INFO] Step 1:         (HAL 2.1.0_0)
[INFO] Step 1:         Compiled Jul 16 2026 09:23:21 (IAR)
[INFO] Step 1: Debug Connection         Enabled
[INFO] Step 1: Debug Notify Transmission Enabled
[INFO] Step 2: SERVER: BLE Stack Initialized
[INFO] Step 2:                 BoardName= SDTR100
[INFO] Step 2:                 BoardMAC = f5:b4:5d:de:fa:3f
[INFO] Step 2:                 BlueNRG-2 HW ver1.2
[INFO] Step 2:                 BlueNRG-2 FW ver2.1.b
[INFO] Step 3: BlueST-SDK V2
[INFO] Step 3: Config  Service added successfully
[INFO] Step 3: Console Service added successfully
[INFO] Step 3: BLE Environmental features ok
[INFO] Step 3: BLE Led features ok
[INFO] Step 3: BLE Sensor Fusion features ok
[INFO] Step 4: Features Service added successfully (Status= 0x0)
[INFO] Step 5: aci_gap_update_adv_data OK
```

## __2. Example configuration__

[![Configuration Manual](doc/configure_with-ConfigurationMa.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/configure/config_toc.html "An offline version is also available in the STM32Cube firmware package.")

This example demonstrates the functionality of the `STM32_BLE_Manager` and `BlueNR2` middlewares.

From the `STM32_BLE_Manager` middleware perspective, SensorDataTransmit example shows how to configure, initialize, and manage the BLE application layer, handling the device state, advertising, connection events, and the interaction between the application logic and the BLE stack to enable sensor data transmission.

In particular, `STM32_BLE_Manager` demonstrates the following components:
- `core/ble_environmental.c/.h`
- `core/ble_led.c/.h`
- `core/ble_sensor_fusion.c/.h`
- `core/ble_manager.c/.h`

The selected components are initialized and added to the Bluetooth characteristics.

From the BlueNRG-2 middleware perspective, SensorDataTransmit shows how to create and use a BLE service to transmit sensor data through GATT and notifications, including the proper initialization and event handling of the BlueNRG-2 stack.

After this step, the firmware is ready to send Bluetooth data according to the enabled characteristics.

### __2.1. Block Diagram__

This example uses the STM32 MCU, the `STM32_BLE_Manager` middleware, and the `X-NUCLEO-BNRG2A1` expansion board to exchange BLE data with the ST BLE Sensor mobile application.

### __2.2. IPs configuration__

__BLE middleware and connectivity__:

Introduction providing a configuration overview:

- UART is enabled for trace output.
- The BLE stack is initialized.
- Environmental, LED, and Sensor Fusion features are registered.
- The device starts advertising and waits for a mobile connection.

The example also requires the project files generated from the proper `.ioc2` configuration.

## __3. Hardware environment and setup__

### __3.1. Generic Setup__

This section describes the hardware setup principles that apply to any board.

### __3.2. Specific board setups__

This section describes the exact hardware configuration used by this example.

<details>
<summary>On STM32C5 series.</summary>

<details>
<summary>On board NUCLEO-C562RE + X-NUCLEO-BNRG2A1.</summary>

| Board connector | MCU pin | Signal name | ARDUINO <br> connector pin | User Label |
| :-------------: | :-----: | :---------: | :------------------------: | :--------: |
| CN-7 | PA6 | SPI1_MISO | D12 | - |
| CN-7 | PA7 | SPI1_MOSI | D11 | - |
| CN-10 | PB3 | SPI1_CLK | D3 | - |
| CN-9 | PA1 | SPI1_CS | A1 | - |
| CN-10 | PA8 | SPI1_RST | D7 | - |
| CN-9 | PA0 | SPI1_EXTI | A0 | - |

</details>
</details>

## __4. Software setup__

To create a functional project, complete the following steps:

- Select the appropriate `.ioc2` file for the target NUCLEO and X-NUCLEO board combination. For example, use `c562re_bnrg2a1_ble_manager_sensor_data_transmit.ioc2` for `NUCLEO-C562RE` with `X-NUCLEO-BNRG2A1`.
- Open the `.ioc2` file with STM32CubeMX.
- Select the preferred toolchain and generate the source code.
- Copy `main.c`, `main.h`, `example.c`, `example.h` and `sensor_data_transmit_config.h` into the generated project folder.
- Open the IDE and add `example.c`, `example.h` and `sensor_data_transmit_config.h` to the project.
- Add `USE_TRACE=1` to the project global defines.
- In **General Options -> Library Configuration**, set **Library low-level interface implementation** to **Semihosted**.
- In the **Linker Script**, set **CSTACK = 0x4000** and **HEAP = 0x5000**.
- Compile the project.


## __5. Troubleshooting__

[![Troubleshooting](doc/debug_with-Troubleshooting.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/debug/debug_toc.html "An offline version is also available in the STM32Cube firmware package.")

Find below the points of attention for this specific example.

__Trace output__: Enable `USE_TRACE=1` if you want to monitor the execution flow through the terminal logs.

__Generated project settings__: After code generation, update the IDE settings as follows:

- In **General Options -> Library Configuration**, set **Library low-level interface implementation** to **Semihosted**.
- In the **Linker Script**, set **CSTACK = 0x4000** and **HEAP = 0x5000**.

## __6. See Also__

[![SeeAlso](doc/go_further_with-STM32.svg)](https://dev.st.com/stm32cube-docs/examples/arch-v1/en/more/more_toc.html "An offline version is also available in the STM32Cube firmware package.")

More information about the `X-NUCLEO-BNRG2A1` expansion board can be found in the [Update procedure and configuration in DTM firmware for X-NUCLEO-BNRG2A1](https://www.st.com/resource/en/application_note/an5651-update-procedure-and-configuration-in-dtm-firmware-for-xnucleobnrg2a1-stmicroelectronics.pdf).

The documentation of the drivers of the relevant STM32 series contains more detailed information.
For instance, for the STM32C5 series: [HAL documentation](https://dev.st.com/stm32cube-docs/stm32c5xx-hal-drivers/latest/en/index.html).

More information about the STM32 ecosystem can be found in the [STM32 MCU Developer Zone](https://www.st.com/content/st_com/en/stm32-mcu-developer-zone/embedded-software.html).

## __7. License__

Copyright (c) 2026 STMicroelectronics.

This software is licensed under terms that can be found in the LICENSE file in the root directory
of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.
