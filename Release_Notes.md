# Release Notes for <mark>stm32cubemx2_ble_manager_sensordatatransmit</mark>

# Purpose

This example demonstrates how to use the STM32_BEL_Manager middleware with STM32CubeMX2.

# Update history

<label for="collapse-v-1-0-0" aria-hidden="true">**1.0.0 / 02-September-2026**</label>
<div>


## Main changes

### First release

Initial release of the SensorDataTransmit example for STM32CubeMX2.

## Known limitations

After code generation:
- open the project options setting and in <b>General Options -> Library Configuration</b> in <b>Library low-level interface implementation</b> set <b>Semihosted</b>
- in the <b>Linker Script</b> set <b>CSTACK = 0x4000</b> and <b>HEAP = 0x5000</b>

## Dependencies

Configuration and code generation features require STM32CubeMX2 Version 1.1.0 or higher.

</div>


For complete documentation on STM32 Microcontrollers,
visit: [STM32 32-bit Arm Cortex MCUs](http://www.st.com/stm32)

<footer class="sticky">
This release note uses up to date web standards and, for this reason, should not
be opened with Internet Explorer but preferably with popular browsers such as
Google Chrome, Mozilla Firefox, Opera or Microsoft Edge.
</footer>
