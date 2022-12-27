/**
 ******************************************************************************
 * @file    main.c
 * @author  Ac6
 * @version V1.0
 * @date    01-December-2013
 * @brief   Default main function.
 *
 * Copyright (c) 2016 Mori
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
 * associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 * LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ******************************************************************************
 *
 * Date: Dez, 26, 2022
 * Modification: - Converted from 'System Workbench for STM32' to 'STM32CubeIDE v1.11.0'
 *                 (See UM2579 - Migration guide from System Workbench to STM32CubeIDE)
 *               - Added DiyMore board (selected in main.h file)
 *                 (See https://stm32-base.org/boards/STM32F407VGT6-diymore.html)
 * By: RTEK1000
 *
 */

#include "stm32f4xx.h"
#include "main.h"

#ifdef DiscoveryBoard
#include "stm32f4_discovery.h"
#endif

#ifdef DiyMoreBoard
#include "stm32f4_diymore.h"
#endif

#include "usbh_core.h"
#include "usbh_hid.h"
#include "usbh_hub.h"

#include "log.h"

USBH_HandleTypeDef hUSBHost[5];

static void USBH_UserProcess(USBH_HandleTypeDef *pHost, uint8_t vId);
static void hub_process();
static void system_clock_config(void);

int main(void) {
	uint32_t i = 0;

	HAL_Init();
	system_clock_config();

#ifdef DiscoveryBoard
	BSP_LED_Init(LED4);
#endif

#ifdef DiyMoreBoard
	BSP_LED_Init(LED2);
#endif

	BSP_UART_Init();

#ifdef DiscoveryBoard
	LOG_INIT(USARTx, 3800000); // 3800000 bauds???
#endif

#ifdef DiyMoreBoard
	LOG_INIT(USARTx, 115200);
#endif

	LOG("\033[2J\033[H");
	LOG(" ");
	LOG("APP RUNNING...");
	LOG("MCU-ID %08X", DBGMCU->IDCODE);

	memset(&hUSBHost[0], 0, sizeof(USBH_HandleTypeDef));

	hUSBHost[0].valid = 1;
	hUSBHost[0].address = USBH_DEVICE_ADDRESS;
	hUSBHost[0].Pipes[0] = USBH_malloc(sizeof(uint32_t) * USBH_MAX_PIPES_NBR);

	USBH_Init(&hUSBHost[0], USBH_UserProcess, ID_USB_HOST_FS);
	USBH_RegisterClass(&hUSBHost[0], USBH_HID_CLASS);
	USBH_RegisterClass(&hUSBHost[0], USBH_HUB_CLASS);

	USBH_Start(&hUSBHost[0]);

	while (1) {
		if (i++ > 150000)
			i = 0;

		if (i > 0 && i <= 25000) {
#ifdef DiscoveryBoard
			BSP_LED_On(LED4);
#endif

#ifdef DiyMoreBoard
			BSP_LED_On(LED2);
#endif
		} else {
#ifdef DiscoveryBoard
			BSP_LED_Off(LED4);
#endif

#ifdef DiyMoreBoard
			BSP_LED_Off(LED2);
#endif
		}

		hub_process();
	}
}

static HID_MOUSE_Info_TypeDef _mouse_info;
static uint32_t mouse_report_data[1];

typedef struct _HID_MOUSE_Info1 {
	uint8_t x;
	uint8_t y;
	uint8_t buttons[14];
} HID_MOUSE_Info1_TypeDef;

HID_MOUSE_Info1_TypeDef _mouse_info1;

/* Structures defining how to access items in a HID mouse report */
/* Access button 1 state. */
static const HID_Report_ItemTypedef prop_b1 = { (uint8_t*) mouse_report_data
		+ 0, /*data*/
1, /*size*/
0, /*shift*/
0, /*count (only for array items)*/
0, /*signed?*/
0, /*min value read can return*/
1, /*max value read can return*/
0, /*min value device can report*/
1, /*max value device can report*/
1 /*resolution*/
};

/* Access button 2 state. */
static const HID_Report_ItemTypedef prop_b2 = { (uint8_t*) mouse_report_data
		+ 0, /*data*/
1, /*size*/
1, /*shift*/
0, /*count (only for array items)*/
0, /*signed?*/
0, /*min value read can return*/
1, /*max value read can return*/
0, /*min value device can report*/
1, /*max value device can report*/
1 /*resolution*/
};

/* Access button 3 state. */
static const HID_Report_ItemTypedef prop_b3 = { (uint8_t*) mouse_report_data
		+ 0, /*data*/
1, /*size*/
2, /*shift*/
0, /*count (only for array items)*/
0, /*signed?*/
0, /*min value read can return*/
1, /*max value read can return*/
0, /*min vale device can report*/
1, /*max value device can report*/
1 /*resolution*/
};

/* Access x coordinate change. */
static const HID_Report_ItemTypedef prop_x = { (uint8_t*) mouse_report_data + 1, /*data*/
8, /*size*/
0, /*shift*/
0, /*count (only for array items)*/
1, /*signed?*/
0, /*min value read can return*/
0xFFFF,/*max value read can return*/
0, /*min vale device can report*/
0xFFFF,/*max value device can report*/
1 /*resolution*/
};

/* Access y coordinate change. */
static const HID_Report_ItemTypedef prop_y = { (uint8_t*) mouse_report_data + 2, /*data*/
8, /*size*/
0, /*shift*/
0, /*count (only for array items)*/
1, /*signed?*/
0, /*min value read can return*/
0xFFFF,/*max value read can return*/
0, /*min vale device can report*/
0xFFFF,/*max value device can report*/
1 /*resolution*/
};

static uint32_t HID_ReadItem1(HID_Report_ItemTypedef *ri, uint8_t ndx) {
	uint32_t val = 0;
	uint32_t x = 0;
	uint32_t bofs;
	uint8_t *data = ri->data;
	uint8_t shift = ri->shift;

	/* get the logical value of the item */

	/* if this is an array, wee may need to offset ri->data.*/
	if (ri->count > 0) {
		/* If app tries to read outside of the array. */
		if (ri->count <= ndx) {
			return (0);
		}

		/* calculate bit offset */
		bofs = ndx * ri->size;
		bofs += shift;
		/* calculate byte offset + shift pair from bit offset. */
		data += bofs / 8;
		shift = (uint8_t) (bofs % 8);
	}
	/* read data bytes in little endian order */
	for (x = 0; x < ((ri->size & 0x7) ? (ri->size / 8) + 1 : (ri->size / 8));
			x++) {
		val = (uint32_t) (*data << (x * 8));
	}
	val = (val >> shift) & ((1 << ri->size) - 1);

	if (val < ri->logical_min || val > ri->logical_max) {
		return (0);
	}

	/* convert logical value to physical value */
	/* See if the number is negative or not. */
	if ((ri->sign) && (val & (1 << (ri->size - 1)))) {
		/* yes, so sign extend value to 32 bits. */
		int vs = (int) ((-1 & ~((1 << (ri->size)) - 1)) | val);

		if (ri->resolution == 1) {
			return ((uint32_t) vs);
		}
		return ((uint32_t) (vs * ri->resolution));
	} else {
		if (ri->resolution == 1) {
			return (val);
		}
		return (val * ri->resolution);
	}
}

static USBH_StatusTypeDef USBH_HID_MouseDecode1(USBH_HandleTypeDef *phost) {
//  HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;
	uint8_t idx = phost->device.current_interface;
	HID_HandleTypeDef *HID_Handle = phost->USBH_ClassTypeDef_pData[idx];

	if (HID_Handle->length[0] == 0) {
		return USBH_FAIL;
	}
	//Fill report
	if (fifo_read(&HID_Handle->fifo, &mouse_report_data, HID_Handle->length[0])
			== HID_Handle->length[0]) {

		if (phost->device.current_interface == 1) {
			LOG("data: %lu", mouse_report_data[0]);
			// LOG("Mini Keyboard + Touchpad");
			// Btn map:
			// 0b00000000000000000000000100000001 - Left
			// 0b00000000000000000000001000000001 - Right
			// 0b01100000000000001110100100000010 - V+ (Up)
			// 0b01100000000000001110101000000010 - V- (Down)
			// 0b01011001000000001011011000000010 - << (Left)
			// 0b01011001000000001011010100000010 - >> (Right)
			// 0b10001111000000011000001100000010 - > (Play)
			// 0b10001001000000011000101000000010 - e (e-mail)
			// 0b01100111000000001110001000000010 - m (Mute)
			// 0b10110110000000100010000100000010 - s (Search)
			// 0b10110111000000100010001100000010 - h (Home)
			// 0b10000001000000011001011000000010 - n (Navigator)
			// 0b00000000000000000000000000000001 - r (Release1)
			// 0b00101111000000000000000000000010 - r (Release2)

			const uint32_t mini_kbd_map[16] = {
					0b00000000000000000000000100000001, // - Left
					0b00000000000000000000001000000001, // - Right
					0b00000000000000000000001100000001, // - Left+Right
					0b01100000000000001110100100000010, // - V+ (Up)
					0b01100000000000001110101000000010, // - V- (Down)
					0b01011001000000001011011000000010, // - << (Left)
					0b01011001000000001011010100000010, // - >> (Right)
					0b01111011000000001100110100000010, // - >| (Play/Pause)
					0b10001111000000011000001100000010, // - > (MediaPlayer)
					0b10001001000000011000101000000010, // - e (e-mail)
					0b01100111000000001110001000000010, // - m (Mute)
					0b10110110000000100010000100000010, // - s (Search)
					0b10110111000000100010001100000010, // - h (Home)
					0b10000001000000011001011000000010, // - n (Navigator)
					0b00000000000000000000000000000001, // - r (Release1)
					0b00101111000000000000000000000010  // - R (Release2)
					};

			// Moves:
			// 0b00000000000000000000000000000001, // - r (Release1)
			// 0b11111111000000000000000000000001
			// 0b00000000111111110000000000000001

			if ((mouse_report_data[0] & 1) == 1) {
				_mouse_info1.x = (mouse_report_data[0] >> 16) & 0xFF;
				_mouse_info1.y = (mouse_report_data[0] >> 24) & 0xFF;

				if(((mouse_report_data[0] >> 8) & 3) == 3) {
					_mouse_info1.buttons[0] = 0;
					_mouse_info1.buttons[1] = 0;
					_mouse_info1.buttons[2] = 1;
				} else {
					_mouse_info1.buttons[0] = (mouse_report_data[0] >> 8) & 1;
					_mouse_info1.buttons[1] = (mouse_report_data[0] >> 9) & 1;
					_mouse_info1.buttons[2] = 0;
				}
			} else {
				for (int i = 2; i < sizeof(_mouse_info1.buttons); i++) {
					if (mouse_report_data[0] == mini_kbd_map[i]) {
						_mouse_info1.buttons[i] = 1;
						// break;
					} else {
						_mouse_info1.buttons[i] = 0;
					}
				}
			}
		} else {
			_mouse_info1.x = (int16_t) HID_ReadItem1(
					(HID_Report_ItemTypedef*) &prop_x, 0);
			_mouse_info1.y = (int16_t) HID_ReadItem1(
					(HID_Report_ItemTypedef*) &prop_y, 0);

			_mouse_info1.buttons[0] = (uint8_t) HID_ReadItem1(
					(HID_Report_ItemTypedef*) &prop_b1, 0);
			_mouse_info1.buttons[1] = (uint8_t) HID_ReadItem1(
					(HID_Report_ItemTypedef*) &prop_b2, 0);
			_mouse_info1.buttons[2] = (uint8_t) HID_ReadItem1(
					(HID_Report_ItemTypedef*) &prop_b3, 0);
		}

		return USBH_OK;
	}

	return USBH_FAIL;
}

static HID_MOUSE_Info1_TypeDef* USBH_HID_GetMouseInfo1(
		USBH_HandleTypeDef *phost) {
	if (USBH_HID_GetDeviceType(phost) == HID_MOUSE) {
		if (USBH_HID_MouseDecode1(phost) == USBH_OK)
			return &_mouse_info1;
	}

	return NULL;
}

void hub_process() {
	static uint8_t current_loop = -1;
	static USBH_HandleTypeDef *_phost = 0;

	if (_phost != NULL && _phost->valid == 1) {
		USBH_Process(_phost);

		if (_phost->busy)
			return;
	}

	while (1) {
		current_loop++;

		if (current_loop > MAX_HUB_PORTS) {
			current_loop = 0;
		}

		if (hUSBHost[current_loop].valid) {
			_phost = &hUSBHost[current_loop];
			USBH_LL_SetupEP0(_phost);

			if (_phost->valid == 3) {
				LOG("PROCESSING ATTACH %d", _phost->address);
				_phost->valid = 1;
				_phost->busy = 1;
			}

			break;
		}
	}

	if (_phost != NULL && _phost->valid) {
		HID_MOUSE_Info1_TypeDef *minfo;
		minfo = USBH_HID_GetMouseInfo1(_phost);

		if (minfo != NULL) {
			LOG("Btn %d%d%d %d%d%d%d%d %d%d%d %d%d%d; X %d; Y %d",
					minfo->buttons[0], minfo->buttons[1], minfo->buttons[2],
					minfo->buttons[3], minfo->buttons[4], minfo->buttons[5],
					minfo->buttons[6], minfo->buttons[7], minfo->buttons[8],
					minfo->buttons[9], minfo->buttons[10], minfo->buttons[11],
					minfo->buttons[12], minfo->buttons[13], minfo->x, minfo->y);
		} else {
			HID_KEYBD_Info_TypeDef *kinfo;
			kinfo = USBH_HID_GetKeybdInfo(_phost);
			if (kinfo != NULL) {
				LOG("KEYB %d", kinfo->keys[0]);
			}
		}
	}
}

void USBH_UserProcess(USBH_HandleTypeDef *pHost, uint8_t vId) {
	switch (vId) {
	case HOST_USER_SELECT_CONFIGURATION:
		break;

	case HOST_USER_CLASS_SELECTED:
		break;

	case HOST_USER_CLASS_ACTIVE:
		break;

	case HOST_USER_CONNECTION:
		break;

	case HOST_USER_DISCONNECTION:
		break;

	case HOST_USER_UNRECOVERED_ERROR:
		break;

	default:
		break;
	}
}

/**
 * @brief  System Clock Configuration
 *         The system Clock is configured as follow :
 *            System Clock source            = PLL (HSE)
 *            SYSCLK(Hz)                     = 168000000
 *            HCLK(Hz)                       = 168000000
 *            AHB Prescaler                  = 1
 *            APB1 Prescaler                 = 4
 *            APB2 Prescaler                 = 2
 *            HSE Frequency(Hz)              = 8000000
 *            PLL_M                          = 8
 *            PLL_N                          = 336
 *            PLL_P                          = 2
 *            PLL_Q                          = 7
 *            VDD(V)                         = 3.3
 *            Main regulator output voltage  = Scale1 mode
 *            Flash Latency(WS)              = 5
 * @param  None
 * @retval None
 */
void system_clock_config(void) {
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;

	/* Enable Power Control clock */
	__PWR_CLK_ENABLE()
	;

	/* The voltage scaling allows optimizing the power consumption when the device is
	 clocked below the maximum system frequency, to update the voltage scaling value
	 regarding system frequency refer to product datasheet.  */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	/* Enable HSE Oscillator and activate PLL with HSE as source */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;

	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_14);
	}

	/* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK) {
		HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_13 | GPIO_PIN_14);
	}
}
