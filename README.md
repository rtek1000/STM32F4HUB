# STM32F4HUB
Add USB Hub support to your STM32F4 project. Based on STM32Cube Library

* Support to Full speed and low speed usb devices through a hub. 
* Support to all hid interfaces.
* Easilly portable to others STM32Fx microcontrollers

* Ready to be opened and compiled with AC6 IDE

It's ready to run on a STM32F4DISCOVERY development board. You just need a Micro-AB to Type A usb cable.

----

- Date: Dez, 26, 2022
- - Modification:
- - - Converted from 'System Workbench for STM32' to 'STM32CubeIDE v1.11.0'
- - - - (See UM2579 - Migration guide from System Workbench to STM32CubeIDE)
- - - Added DiyMore board (selected in main.h file)
- - - - (See https://stm32-base.org/boards/STM32F407VGT6-diymore.html)
- - By: RTEK1000
-
- Date: Dez, 27, 2022
- - Modification:
- - - Added Alternative support for Mini Keyboard (built-in touchpad)
- - - - (See https://www.amazon.com/Wireless-Keyboard-BIFANS-Portable-Touchpad/dp/B075SBX1CX)
- - - - ![image](https://raw.githubusercontent.com/rtek1000/STM32F4HUB/master/Project-STM32CubeIDE_Mini_Keyboard/Mini_Keyboard1.jpg)
- - By: RTEK1000
-
- Date: Dez, 31, 2022
- - Modification:
- - - Added support for two USB ports
- - - Added MSC class support
- - - Modified HUB Class
- - By: RTEK1000

- Date: Jun, 13, 2023
- - Debug info
- - By: RTEK1000

Debug message from USB Host Stack without HUB support:
```
USB Device Connected
USB Device Reset Completed
PID: 318h
VID: 513h
Address (#1) assigned.
Manufacturer : N/A
Product : N/A
Serial Number : N/A
Enumeration done.
This device has only 1 configuration.
Default configuration set.
Device remote wakeup enabled
Switching to Interface (#0)
Class    : 3h
SubClass : 1h
Protocol : 1h
KeyBoard device found!
HID class started.
```
Debug message from USB Host Stack with HUB support:
```
APP RUNNING...
MCU-ID 10076413
USBH_LL_GetSpeed 2 (LOW=2,FULL=1)
addrd: 0x00, pipe: 0
addrd: 0x80, pipe: 1
USBH_ParseDevDesc
12 01 10 01 00 00 00 08
Enum mps: 8, addr: 0
USBH_ParseDevDesc
12 01 10 01 00 00 00 08 13 05 18 03 00 01 00 00 00 01
PID  : 318h
VID  : 513h
Address (#5) assigned.
USBH_ParseCfgDesc, number of interfaces: 2
itfclass 0x03
USBH_ParseInterfaceDesc, itf 0, class 0x03, subclass: 0x01, proto: 0x01, numep 1
itfclass 0x03
USBH_ParseInterfaceDesc, itf 1, class 0x03, subclass: 0x01, proto: 0x02, numep 1
Manufacturer : N/A
Product : N/A
Serial Number : N/A
This device has only 1 configuration.
HID_KEYBRD_BOOT_CODE
Default configuration set.
INTERFACES: 2
Switching to Interface (#0)
Class    : 3h
SubClass : 1h
Protocol : 1h
HID LEN 64 - 0
KeyBoard found.
addrd: 0x81, pipe: 2
IN  size=8, num 2, addr 0x81, pool 8, lowspeed? 1
Switching to Interface (#1)
Class    : 3h
SubClass : 1h
Protocol : 2h
HID LEN 119 - 1
Mouse found.
addrd: 0x82, pipe: 3
IN  size=8, num 3, addr 0x82, pool 8, lowspeed? 1
Switching to Interface (#0)
HID class started.
data: 1
Btn 000 00000 000 000; X 0; Y 0
KEYB 0
data: 788529154
Btn 000 00000 000 000; X 0; Y 0
```

