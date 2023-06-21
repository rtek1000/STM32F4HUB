# STM32F4HUB

- Note: Modified projects were migrated to this repository: [STM32F4HUB_modified](https://github.com/rtek1000/STM32F4HUB_modified)

------

- Support to all HID interfaces, more info: [STM32CubeIDE](https://github.com/rtek1000/STM32F4HUB_modified) Projects.
- - To recognize the Mini Keyboard with built-in touchpad
- - - This project can serve as a basis for other devices with multiple interfaces
- - - - To do: Update HID project from ASC project (it stopped recognizing the multimedia keyboard)
-
- Date: Jun, 20, 2023
- - Modification:
- - - Adaptation of [Project-STM32CubeIDE_USBH_HID](https://github.com/rtek1000/STM32F4HUB_modified/tree/main/Project-STM32CubeIDE_USBH_HID)
- - - Project "All Supported Classes" automatically generated by STM32CubeIDE version: 1.12.1 (Current version)
- - - - Modifications to USB port RESET timers to not block code for many milliseconds
- - - - Added automatic recovery, "de-initialize" the USB port and "re-initialize"
- - - - Port timeout modifications for automatic recovery
- - - - Adaptation to recognize more HID interfaces
- - - - - Mini Wireless Keyboard Built-in Touchpad  (VID: 0x0513, PID: 0x0318) supported, multimedia buttons and touchpad supported
- - - - - Regular keyboard multimedia interface was not implemented, only the keyboard part works
- - - - Keyboard LED control supported
- - - - Wireless Mouse Logitech M220 (and M185) (VID: 0x046D, PID: 0xC534) supported
- - - - HUB class not implemented
- - - - Folder: [Project-STM32CubeIDE_USBH_ASC](https://github.com/rtek1000/STM32F4HUB_modified/tree/main/Project-STM32CubeIDE_USBH_ASC)
- - By: RTEK1000

-----

## STM32F4HUB - Original info

Add USB Hub support to your STM32F4 project. Based on STM32Cube Library

* Support to Full speed and low speed usb devices through a hub. 
* Support to all hid interfaces.
* Easilly portable to others STM32Fx microcontrollers

* Ready to be opened and compiled with AC6 IDE

It's ready to run on a STM32F4DISCOVERY development board. You just need a Micro-AB to Type A usb cable.
