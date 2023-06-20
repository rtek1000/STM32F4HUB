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
// M.W.K.B.T.: Mini Wireless Keyboard with Built-in Touchpad
typedef enum {
	CODE_BTN_NEXT = 0x00B5,
	CODE_BTN_PREV = 0x00B6,
	CODE_BTN_PLAY_PAUSE = 0x00CD,
	CODE_BTN_MUTE = 0x00E2,
	CODE_BTN_VOL_UP = 0x00E9,
	CODE_BTN_VOL_DOWN = 0x00EA,
	CODE_BTN_MEDIA_PLAYER = 0x0183,
	CODE_BTN_EMAIL = 0x018A,
	CODE_BTN_BROWSER = 0x0196,
	CODE_BTN_SEARCH = 0x0221,
	CODE_BTN_HOME = 0x0223,
} MWKBT1_TypeTypeDef;

typedef enum {
	MWKBT_EMPTY = 0,
	MWKBT_VALID,
	MWKBT_NEXT,
	MWKBT_PREV,
	MWKBT_PLAY_PAUSE,
	MWKBT_MUTE,
	MWKBT_VOL_UP,
	MWKBT_VOL_DOWN,
	MWKBT_MEDIA_PLAYER,
	MWKBT_EMAIL,
	MWKBT_BROWSER,
	MWKBT_SEARCH,
	MWKBT_HOME,
} MWKBT2_TypeTypeDef;

typedef struct _HID_Multimedia_Touchpad {
	uint8_t x;
	uint8_t y;
	uint8_t buttons[3];
	uint8_t multimedia;
	uint8_t valid;
} HID_Multimedia_Touchpad_TypeDef;

HID_Multimedia_Touchpad_TypeDef info_decoded;

HID_KEYBD_Info_TypeDef *keybd_info1;
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/
HID_Multimedia_Touchpad_TypeDef* HID_Decode_Mini_Keyboard_Touchpad(
		HID_MOUSE_Info_TypeDef *Minfo);
/* USER CODE END PFP */

/* USB Host core handle declaration */
USBH_HandleTypeDef hUsbHostFS;
ApplicationTypeDef Appli_state = APPLICATION_IDLE;

/*
 * -- Insert your variables declaration here --
 */
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*
 * user callback declaration
 */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);

/*
 * -- Insert your external function declaration here --
 */
/* USER CODE BEGIN 1 */
// Mini Wireless Keyboard with built-in Touchpad decode:
HID_Multimedia_Touchpad_TypeDef* HID_Decode_Mini_Keyboard_Touchpad(
		HID_MOUSE_Info_TypeDef *Minfo) {
	info_decoded.valid = MWKBT_EMPTY;
	info_decoded.multimedia = 0;

	info_decoded.buttons[0] = 0;
	info_decoded.buttons[1] = 0;
	info_decoded.buttons[2] = 0;

	uint8_t raw_length = Minfo->raw_length;
	uint8_t raw_data0 = Minfo->raw_data32[1] >> 16;

	if ((raw_length == 8) && (raw_data0 == 0x08)) {
		if ((((Minfo->raw_data32[0] >> 1) & 1) == 0)
				&& ((Minfo->raw_data32[0] & 1) == 1)) {

//			printf("Mouse functions\n");

			uint8_t btn0 = (Minfo->raw_data32[0] >> 8) & 1;
			uint8_t btn1 = (Minfo->raw_data32[0] >> 9) & 1;

			info_decoded.buttons[0] = btn0;
			info_decoded.buttons[1] = btn1;

			if ((info_decoded.buttons[0] == 1)
					&& (info_decoded.buttons[1] == 1)) {
				info_decoded.buttons[0] = 0;
				info_decoded.buttons[1] = 0;
				info_decoded.buttons[2] = 1;
			}

			uint8_t x_val = (Minfo->raw_data32[0] >> 16) & 0xFF;
			uint8_t y_val = (Minfo->raw_data32[0] >> 24) & 0xFF;

			if ((x_val > 0) || (y_val > 0)) {
				info_decoded.x = x_val;
				info_decoded.y = y_val;
			}

			if ((btn0 != 0) || (btn1 != 0) || (x_val > 0) || (y_val > 0)) {
				info_decoded.valid = MWKBT_VALID;
			}
		} else if ((((Minfo->raw_data32[0] >> 1) & 1) == 1)
				&& ((Minfo->raw_data32[0] & 1) == 0)) {

//			printf("Multimedia functions\n");

			uint16_t val1 = (Minfo->raw_data32[0] >> 8) & 0xFFFF;

//			printf("val1: 0x%04X\n", val1);

			switch (val1) {
			case CODE_BTN_NEXT:
				info_decoded.multimedia = MWKBT_NEXT;
				break;
			case CODE_BTN_PREV:
				info_decoded.multimedia = MWKBT_PREV;
				break;
			case CODE_BTN_PLAY_PAUSE:
				info_decoded.multimedia = MWKBT_PLAY_PAUSE;
				break;
			case CODE_BTN_MUTE:
				info_decoded.multimedia = MWKBT_MUTE;
				break;
			case CODE_BTN_VOL_UP:
				info_decoded.multimedia = MWKBT_VOL_UP;
				break;
			case CODE_BTN_VOL_DOWN:
				info_decoded.multimedia = MWKBT_VOL_DOWN;
				break;
			case CODE_BTN_MEDIA_PLAYER:
				info_decoded.multimedia = MWKBT_MEDIA_PLAYER;
				break;
			case CODE_BTN_EMAIL:
				info_decoded.multimedia = MWKBT_EMAIL;
				break;
			case CODE_BTN_BROWSER:
				info_decoded.multimedia = MWKBT_BROWSER;
				break;
			case CODE_BTN_SEARCH:
				info_decoded.multimedia = MWKBT_SEARCH;
				break;
			case CODE_BTN_HOME:
				info_decoded.multimedia = MWKBT_HOME;
				break;
			default:

				break;
			}

			if (info_decoded.multimedia != MWKBT_EMPTY) {
				info_decoded.valid = MWKBT_VALID;
			}
		}
	}

	return &info_decoded;
}

void USBH_HID_EventCallback(USBH_HandleTypeDef *phost) {
	uint8_t idx = phost->device.current_interface;

	HID_HandleTypeDef *HID_Handle =
			(HID_HandleTypeDef*) phost->pActiveClass->pData_array[idx];
	if (HID_Handle->Init == USBH_HID_KeybdInit) {
		keybd_info1 = USBH_HID_GetKeybdInfo(phost);

		if ((keybd_info1->key_ascii != 0U) && (keybd_info1->key_ascii != 0x0AU)
				&& (keybd_info1->keys[0] != KEY_HOME)) {
			if (keybd_info1->keys[0] == KEY_BACKSPACE) {
				USBH_UsrLog("ASCII: [BS]; Hex: 0x%02X; (Keycode: %02X)",
						keybd_info1->key_ascii, keybd_info1->keys[0]);
			} else if (keybd_info1->keys[0] == KEY_ESCAPE) {
				USBH_UsrLog("ASCII: [ESC]; Hex: 0x%02X; (Keycode: %02X)",
						keybd_info1->key_ascii, keybd_info1->keys[0]);
			} else if (keybd_info1->keys[0] == KEY_TAB) {
				USBH_UsrLog("ASCII: [TAB]; Hex: 0x%02X; (Keycode: %02X)",
						keybd_info1->key_ascii, keybd_info1->keys[0]);
			} else if (keybd_info1->keys[0] == KEY_SPACEBAR) {
				USBH_UsrLog("ASCII: [SPC]; Hex: 0x%02X; (Keycode: %02X)",
						keybd_info1->key_ascii, keybd_info1->keys[0]);
			} else {
				USBH_UsrLog("ASCII: [%c]; Hex: 0x%02X; (Keycode: %02X)",
						keybd_info1->key_ascii, keybd_info1->key_ascii,
						keybd_info1->keys[0]);
			}
		} else if (keybd_info1->keys[0] != 0) {
			if ((keybd_info1->keys[0] >= KEY_F1)
					&& (keybd_info1->keys[0] <= KEY_F12)) {
				USBH_UsrLog("Key: [F%d]; Hex: 0x%02X; (Keycode: %02X)",
						keybd_info1->keys[0] - KEY_F1, keybd_info1->key_ascii,
						keybd_info1->keys[0]);
			} else {
				char str1[20];

				switch (keybd_info1->keys[0]) {
				case KEY_CAPS_LOCK:
					sprintf(str1, "Caps");
					break;
				case KEY_ENTER:
				case KEY_KEYPAD_ENTER:
					sprintf(str1, "Enter");
					break;
				case KEY_PAGEUP:
				case KEY_KEYPAD_9_PAGEUP:
					sprintf(str1, "PgUp");
					break;
				case KEY_PAGEDOWN:
				case KEY_KEYPAD_3_PAGEDN:
					sprintf(str1, "PgDn");
					break;
				case KEY_DELETE:
					sprintf(str1, "Del");
					break;
				case KEY_END1:
				case KEY_KEYPAD_1_END:
					sprintf(str1, "End");
					break;
				case KEY_PRINTSCREEN:
					sprintf(str1, "PS");
					break;
				case KEY_UPARROW:
				case KEY_KEYPAD_8_UP_ARROW:
					sprintf(str1, "Up");
					break;
				case KEY_DOWNARROW:
				case KEY_KEYPAD_2_DOWN_ARROW:
					sprintf(str1, "Dn");
					break;
				case KEY_RIGHTARROW:
				case KEY_KEYPAD_6_RIGHT_ARROW:
					sprintf(str1, "Rg");
					break;
				case KEY_LEFTARROW:
				case KEY_KEYPAD_4_LEFT_ARROW:
					sprintf(str1, "Lf");
					break;
				case KEY_HOME:
				case KEY_KEYPAD_7_HOME:
					sprintf(str1, "Home");
					break;
				case KEY_INSERT:
				case KEY_KEYPAD_0_INSERT:
					sprintf(str1, "Ins");
					break;
				case KEY_PAUSE:
					sprintf(str1, "Pause");
					break;
				case KEY_KEYPAD_NUM_LOCK_AND_CLEAR:
					sprintf(str1, "NumLk");
					break;
				default:
					str1[0] = 0;
					break;
				}

				if (str1[0] != 0) {
					USBH_UsrLog("Key: [%s]; Hex: 0x%02X; (Keycode: %02X)", str1,
							keybd_info1->key_ascii, keybd_info1->keys[0]);
				} else {
					USBH_UsrLog(
							"KB action: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x",
							keybd_info1->keys[0], keybd_info1->keys[1],
							keybd_info1->keys[2], keybd_info1->keys[3],
							keybd_info1->keys[4], keybd_info1->keys[5]); // USBH_DbgLog
				}
			}
		}

		if (keybd_info1->key_special == 1U) {
			USBH_UsrLog(
					"lalt: %d, ralt: %d, lctrl: %d, rctrl: %d, lgui: %d, rgui: %d, lshift: %d, rshift: %d, state: %d",
					keybd_info1->lalt, keybd_info1->ralt, keybd_info1->lctrl,
					keybd_info1->rctrl, keybd_info1->lgui, keybd_info1->rgui,
					keybd_info1->lshift, keybd_info1->rshift,
					keybd_info1->state);
		}
	} else if (HID_Handle->Init == USBH_HID_MouseInit) {
		HID_MOUSE_Info_TypeDef *mouse_info;

		mouse_info = USBH_HID_GetMouseInfo(phost);

		if (mouse_info->raw_length == 0x08) { // mini wireless keyboard
			HID_Multimedia_Touchpad_TypeDef *mini_kb_touchpad;

			mini_kb_touchpad = HID_Decode_Mini_Keyboard_Touchpad(mouse_info);

			if (mini_kb_touchpad->valid == MWKBT_VALID) {
				if (mini_kb_touchpad->multimedia != MWKBT_EMPTY) {
					switch (mini_kb_touchpad->multimedia) {
					case MWKBT_NEXT:
						USBH_UsrLog("Next");
						break;
					case MWKBT_PREV:
						USBH_UsrLog("Previous");
						break;
					case MWKBT_PLAY_PAUSE:
						USBH_UsrLog("Play/Pause");
						break;
					case MWKBT_MUTE:
						USBH_UsrLog("Mute");
						break;
					case MWKBT_VOL_UP:
						USBH_UsrLog("Volume Up");
						break;
					case MWKBT_VOL_DOWN:
						USBH_UsrLog("Volume Down");
						break;
					case MWKBT_MEDIA_PLAYER:
						USBH_UsrLog("Media Player");
						break;
					case MWKBT_EMAIL:
						USBH_UsrLog("e-mail");
						break;
					case MWKBT_BROWSER:
						USBH_UsrLog("Browser");
						break;
					case MWKBT_SEARCH:
						USBH_UsrLog("Search");
						break;
					case MWKBT_HOME:
						USBH_UsrLog("Home");
						break;
					default:

						break;
					}
				} else {
					if (mini_kb_touchpad->valid == MWKBT_VALID) {
						USBH_UsrLog(
								"Mini KB Touchpad action: x=  0x%x, y=  0x%x, button1= 0x%x, button2= 0x%x, button3= 0x%x",
								mini_kb_touchpad->x, mini_kb_touchpad->y,
								mini_kb_touchpad->buttons[0],
								mini_kb_touchpad->buttons[1],
								mini_kb_touchpad->buttons[2]);
					}
				}
			}
		} else { // regular mouse
			//printf("Mouse action (raw data): ");

			//print_raw_info(mouse_info);

			USBH_UsrLog(
					"Mouse action: x=  0x%x, y=  0x%x, button1= 0x%x, button2= 0x%x, button3= 0x%x",
					mouse_info->x, mouse_info->y, mouse_info->buttons[0],
					mouse_info->buttons[1], mouse_info->buttons[2]);
		}
	}
}
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

