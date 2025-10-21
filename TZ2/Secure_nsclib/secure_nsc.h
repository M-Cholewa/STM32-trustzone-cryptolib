/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    Secure_nsclib/secure_nsc.h
  * @author  MCD Application Team
  * @brief   Header for secure non-secure callable APIs list
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

/* USER CODE BEGIN Non_Secure_CallLib_h */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SECURE_NSC_H
#define SECURE_NSC_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "aes_encrypt_decrypt.h"

/* Exported types ------------------------------------------------------------*/
/**
  * @brief  non-secure callback ID enumeration definition
  */
typedef enum
{
  SECURE_FAULT_CB_ID     = 0x00U, /*!< System secure fault callback ID */
  GTZC_ERROR_CB_ID       = 0x01U  /*!< GTZC secure error callback ID */
} SECURE_CallbackIDTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SECURE_RegisterCallback(SECURE_CallbackIDTypeDef CallbackId, void *func);
void SECURE_Set_Led(void);
void SECURE_Reset_Led(void);


/* Public macros -------------------------------------------------------------*/

/* Public typedef ------------------------------------------------------------*/

/* NSC wrapper functions */
aes_status_t SECURE_AES_Crypto_Init(void);
aes_status_t SECURE_AES_Crypto_Finalize(void);

aes_status_t SECURE_AES_GCM_Encrypt(
    const uint8_t *plaintext, size_t plaintext_len,
    uint8_t *ciphertext_and_tag, size_t *output_len);

aes_status_t SECURE_AES_GCM_Decrypt(
    const uint8_t *ciphertext_and_tag, size_t input_len,
    uint8_t *plaintext, size_t *plaintext_len);
#endif /* SECURE_NSC_H */
/* USER CODE END Non_Secure_CallLib_h */

