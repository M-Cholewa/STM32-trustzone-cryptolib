/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    Secure/Src/secure_nsc.c
 * @author  MCD Application Team
 * @brief   This file contains the non-secure callable APIs (secure world)
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
/* USER CODE END Header */

/* USER CODE BEGIN Non_Secure_CallLib */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "secure_nsc.h"
/** @addtogroup STM32H5xx_HAL_Examples

 * @{
 */

/** @addtogroup Templates
 * @{
 */

/* Global variables ----------------------------------------------------------*/
void *pSecureFaultCallback = NULL; /* Pointer to secure fault callback in Non-secure */
void *pSecureErrorCallback = NULL; /* Pointer to secure error callback in Non-secure */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 * @brief  Secure registration of non-secure callback.
 * @param  CallbackId  callback identifier
 * @param  func        pointer to non-secure function
 * @retval None
 */
CMSE_NS_ENTRY void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func)
{
	if ( func != NULL )
	{
		switch ( CallbackId )
		{
			case SECURE_FAULT_CB_ID: /* SecureFault Interrupt occurred */
				pSecureFaultCallback = func;
				break;
			case GTZC_ERROR_CB_ID: /* GTZC Interrupt occurred */
				pSecureErrorCallback = func;
				break;
			default:
				/* unknown */
				break;
		}
	}
}

CMSE_NS_ENTRY void SECURE_Set_Led(void)
{
	HAL_GPIO_WritePin( LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET );
}

CMSE_NS_ENTRY void SECURE_Reset_Led(void)
{
	HAL_GPIO_WritePin( LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET );
}

__attribute__((cmse_nonsecure_entry))
aes_status_t SECURE_AES_Crypto_Init(void)
{
    return AES_Crypto_Init();
}

__attribute__((cmse_nonsecure_entry))
aes_status_t SECURE_AES_Crypto_Finalize(void)
{
    return AES_Crypto_Finalize();
}

static const uint8_t key[AES_KEY_SIZE_256] = {
  0x46,0x3b,0x41,0x29,0x11,0x76,0x7d,0x57,
  0xa0,0xb3,0x39,0x69,0xe6,0x74,0xff,0xe7,
  0x84,0x5d,0x31,0x3b,0x88,0xc6,0xfe,0x31,
  0x2f,0x3d,0x72,0x4b,0xe6,0x8e,0x1f,0xca
};

static const uint8_t iv[AES_IV_SIZE] = {
  0x61,0x1c,0xe6,0xf9,0xa6,0x88,0x07,0x50,0xde,0x7d,0xa6,0xcb
};

static const uint8_t aad[] = { 0x01, 0x02, 0x03, 0x04 };

__attribute__((cmse_nonsecure_entry))
aes_status_t SECURE_AES_GCM_Encrypt(
    const uint8_t *plaintext, size_t plaintext_len,
    uint8_t *ciphertext_and_tag, size_t *output_len)
{
    return AES_GCM_Encrypt(
        plaintext, plaintext_len,
        key, sizeof(key),
        iv, sizeof(iv),
        aad, sizeof(aad),
        ciphertext_and_tag, output_len);
}

__attribute__((cmse_nonsecure_entry))
aes_status_t SECURE_AES_GCM_Decrypt(
    const uint8_t *ciphertext_and_tag, size_t input_len,
    uint8_t *plaintext, size_t *plaintext_len)
{
    return AES_GCM_Decrypt(
        ciphertext_and_tag, input_len,
        key, sizeof(key),
        iv, sizeof(iv),
        aad, sizeof(aad),
        plaintext, plaintext_len);
}
/**
 * @}
 */

/**
 * @}
 */
/* USER CODE END Non_Secure_CallLib */

