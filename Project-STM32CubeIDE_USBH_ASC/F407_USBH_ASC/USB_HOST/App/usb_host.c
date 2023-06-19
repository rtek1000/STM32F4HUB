/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file            : usb_host.c
 * @version         : v1.0_Cube
 * @brief           : This file implements the USB Host
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/

#include "usb_host.h"
#include "usbh_core.h"
#include "usbh_audio.h"
#include "usbh_cdc.h"
#include "usbh_msc.h"
#include "usbh_hid.h"
#include "usbh_mtp.h"
#include "usbh_hub.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */
HID_KEYBD_Info_TypeDef *keybd_info1;
extern HID_MOUSE_Info_TypeDef mouse_info;
/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */
void USBH_HID_EventCallback(USBH_HandleTypeDef *phost) {
	uint8_t idx = phost->device.current_interface;

	HID_HandleTypeDef *HID_Handle =
			(HID_HandleTypeDef*) phost->pActiveClass->pData_array[idx];
	if (HID_Handle->Init == USBH_HID_KeybdInit) {
		keybd_info1 = USBH_HID_GetKeybdInfo(phost);

		USBH_UsrLog("KB action: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x",
				keybd_info1->keys[0], keybd_info1->keys[1],
				keybd_info1->keys[2], keybd_info1->keys[3],
				keybd_info1->keys[4], keybd_info1->keys[5]); // USBH_DbgLog

//		if ((keybd_info1->key_ascii != 0U) && (keybd_info1->key_ascii != 0x0AU)) {
//			USBH_UsrLog("ASCII: %c; Hex: 0x%02X; (Keycode: %02X)",
//					keybd_info1->key_ascii, keybd_info1->key_ascii,
//					keybd_info1->keys[0]);
//		}
//
//		if (keybd_info1->key_special == 1U) {
//			USBH_UsrLog(
//					"lalt: %d, ralt: %d, lctrl: %d, rctrl: %d, lgui: %d, rgui: %d, lshift: %d, rshift: %d, state: %d",
//					keybd_info1->lalt, keybd_info1->ralt, keybd_info1->lctrl,
//					keybd_info1->rctrl, keybd_info1->lgui, keybd_info1->rgui,
//					keybd_info1->lshift, keybd_info1->rshift,
//					keybd_info1->state);
//		}
	} else if (HID_Handle->Init == USBH_HID_MouseInit) {
		USBH_HID_GetMouseInfo(phost);

		USBH_UsrLog(
				"Mouse action: x=  0x%x, y=  0x%x, button1= 0x%x, button2= 0x%x, button3= 0x%x",
				mouse_info.x, mouse_info.y, mouse_info.buttons[0],
				mouse_info.buttons[1], mouse_info.buttons[2]);
	}
}

//static USBH_HandleTypeDef *_phost = 0;
//
//static void hub_process(USBH_HandleTypeDef *hUSBHost) {
//	static uint8_t current_loop = -1;
//
//	if (hUSBHost != NULL && hUSBHost->valid == 1) {
//		//USBH_DbgLog("USBH_Process");
//		hUSBHost->id = hUSBHost->id;
//		USBH_Process(hUSBHost);
//
//		if (hUSBHost->busy)
//			return;
//	}
//
//	while (1) {
//		current_loop++;
//
//		if (current_loop > MAX_HUB_PORTS) {
//			current_loop = 0;
//		}
//
//		if (hUSBHost[current_loop].valid) {
//			_phost = &hUSBHost[current_loop];
//			//USBH_DbgLog("USBH_LL_SetupEP0");
//			USBH_LL_SetupEP0(_phost);
//
//			if (_phost->valid == 3) {
//				USBH_DbgLog("PROCESSING ATTACH %d", _phost->address);
//				_phost->valid = 1;
//				_phost->busy = 1;
//			}
//
//			break;
//		}
//	}
//
//	if (_phost != NULL && _phost->valid) {
//		HID_MOUSE_Info_TypeDef *minfo;
//		minfo = USBH_HID_GetMouseInfo(_phost);
//
//		if (minfo != NULL) {
//			USBH_DbgLog("Btn %d%d%d %d%d%d%d%d %d%d%d %d%d%d; X %d; Y %d",
//					minfo->buttons[0], minfo->buttons[1], minfo->buttons[2],
//					minfo->buttons[3], minfo->buttons[4], minfo->buttons[5],
//					minfo->buttons[6], minfo->buttons[7], minfo->buttons[8],
//					minfo->buttons[9], minfo->buttons[10], minfo->buttons[11],
//					minfo->buttons[12], minfo->buttons[13], minfo->x, minfo->y);
//		} else {
//			HID_KEYBD_Info_TypeDef *kinfo;
//			kinfo = USBH_HID_GetKeybdInfo(_phost);
//			if (kinfo != NULL) {
//				uint8_t key = kinfo->keys[0];
//
//				if (key == KEY_INSERT) {
//					keyboard_insert = !keyboard_insert;
//				} else {
//					//if((HAL_GetTick() - keyboard_led_timeout) > 300){
//						keyboard_led_timeout = HAL_GetTick();
//
//						if (key == KEY_KEYPAD_NUM_LOCK_AND_CLEAR) {
//							if ((kbd_LED_status[0] & 1) == 0) {
//								kbd_LED_status[0] |= 0B001;
//							} else {
//								kbd_LED_status[0] &= 0B110;
//							}
//
//							USBH_DbgLog("kbd_LED_status %d", kbd_LED_status[0]);
//
//							//		USB_Set_Keyboard_LED_Status();
//						} else if (key == KEY_CAPS_LOCK) {
//							if (((kbd_LED_status[0] >> 1) & 1) == 0) {
//								kbd_LED_status[0] |= 0B010;
//							} else {
//								kbd_LED_status[0] &= 0B101;
//							}
//
//							USBH_DbgLog("kbd_LED_status %d", kbd_LED_status[0]);
//						} else if (key == KEY_SCROLL_LOCK) {
//							if (((kbd_LED_status[0] >> 2) & 1) == 0) {
//								kbd_LED_status[0] |= 0B100;
//							} else {
//								kbd_LED_status[0] &= 0B011;
//							}
//
//							USBH_DbgLog("kbd_LED_status %d", kbd_LED_status[0]);
//						}
//					//} else {
//						// keyboardMain(keybd_info1);
//					//}
//				}
//
//				USBH_DbgLog("KEYB %d", kinfo->keys[0]);
//			}
//		}
//	}
//}
/* USER CODE END 1 */

/**
 * Init USB host library, add supported class and start the library
 * @retval None
 */
void MX_USB_HOST_Init(void) {
	/* USER CODE BEGIN USB_HOST_Init_PreTreatment */
#ifdef HOST_HS
	// stm32f4xx_hal_rcc_ex.h
	__HAL_RCC_USB_OTG_HS_FORCE_RESET();
	__HAL_RCC_USB_OTG_HS_RELEASE_RESET();
#endif

#ifdef HOST_FS
	// stm32f4xx_hal_rcc_ex.h
	__HAL_RCC_USB_OTG_FS_FORCE_RESET();
	__HAL_RCC_USB_OTG_FS_RELEASE_RESET();
#endif
	/* USER CODE END USB_HOST_Init_PreTreatment */

	/* Init host Library, add supported class and start the library. */
	if (USBH_Init(&hUsbHostFS, USBH_UserProcess, HOST_FS) != USBH_OK) {
		Error_Handler();
	}
	if (USBH_RegisterClass(&hUsbHostFS, USBH_AUDIO_CLASS) != USBH_OK) {
		Error_Handler();
	}
	if (USBH_RegisterClass(&hUsbHostFS, USBH_CDC_CLASS) != USBH_OK) {
		Error_Handler();
	}
	if (USBH_RegisterClass(&hUsbHostFS, USBH_MSC_CLASS) != USBH_OK) {
		Error_Handler();
	}
	if (USBH_RegisterClass(&hUsbHostFS, USBH_HID_CLASS) != USBH_OK) {
		Error_Handler();
	}
	if (USBH_RegisterClass(&hUsbHostFS, USBH_MTP_CLASS) != USBH_OK) {
		Error_Handler();
	}
	if (USBH_Start(&hUsbHostFS) != USBH_OK) {
		Error_Handler();
	}
	/* USER CODE BEGIN USB_HOST_Init_PostTreatment */

	/* USER CODE END USB_HOST_Init_PostTreatment */
}

/*
 * Background task
 */
void MX_USB_HOST_Process(void) {
	/* USB Host Background task */
	USBH_Process(&hUsbHostFS);
}
/*
 * user callback definition
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id) {
	/* USER CODE BEGIN CALL_BACK_1 */
	switch (id) {
	case HOST_USER_SELECT_CONFIGURATION:
		break;

	case HOST_USER_DISCONNECTION:
		Appli_state = APPLICATION_DISCONNECT;
		break;

	case HOST_USER_CLASS_ACTIVE:
		Appli_state = APPLICATION_READY;
		break;

	case HOST_USER_CONNECTION:
		Appli_state = APPLICATION_START;
		break;

	default:
		break;
	}
	/* USER CODE END CALL_BACK_1 */
}

/**
 * @}
 */

/**
 * @}
 */

