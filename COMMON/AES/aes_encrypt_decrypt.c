/*
 * aes_encrypt_decrypt.c
 *
 *  Created on: Oct 18, 2025
 *      Author: adminn
 */

/********************** NOTES *************************************************
 ...
 *******************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "aes_encrypt_decrypt.h"
#include "stdint.h"
#include "cmox_crypto.h"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//#ifdef USERLIB_USE_ENABLED // plik.c
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/* Private define ------------------------------------------------------------*/

/* Private macros ------------------------------------------------------------*/

/* Private typedef -----------------------------------------------------------*/

/* Private constants ---------------------------------------------------------*/

/* Private variables ---------------------------------------------------------*/

/* Private function declaration ----------------------------------------------*/

/* Public functions definition -----------------------------------------------*/
aes_status_t AES_Crypto_Init(void)
{
    cmox_init_arg_t init_target = { CMOX_INIT_TARGET_AUTO, NULL };
    cmox_init_retval_t ret = cmox_initialize(&init_target);

    return (ret == CMOX_INIT_SUCCESS) ? AES_SUCCESS : AES_ERROR;
}

aes_status_t AES_Crypto_Finalize(void)
{
    cmox_finalize(NULL);
    return AES_SUCCESS;
}

aes_status_t AES_GCM_Encrypt(
    const uint8_t *plaintext, size_t plaintext_len,
    const uint8_t *key, size_t key_len,
    const uint8_t *iv, size_t iv_len,
    const uint8_t *aad, size_t aad_len,
    uint8_t *ciphertext_and_tag, size_t *output_len)
{
    cmox_cipher_retval_t ret = cmox_aead_encrypt(
        CMOX_AES_GCM_ENC_ALGO,
        plaintext, plaintext_len,
        AES_TAG_SIZE,
        key, key_len,
        iv, iv_len,
        aad, aad_len,
        ciphertext_and_tag, output_len);

    return (ret == CMOX_CIPHER_SUCCESS) ? AES_SUCCESS : AES_ERROR;
}

aes_status_t AES_GCM_Decrypt(
    const uint8_t *ciphertext_and_tag, size_t input_len,
    const uint8_t *key, size_t key_len,
    const uint8_t *iv, size_t iv_len,
    const uint8_t *aad, size_t aad_len,
    uint8_t *plaintext, size_t *plaintext_len)
{
    cmox_cipher_retval_t ret = cmox_aead_decrypt(
        CMOX_AES_GCM_DEC_ALGO,
        ciphertext_and_tag, input_len,
        AES_TAG_SIZE,
        key, key_len,
        iv, iv_len,
        aad, aad_len,
        plaintext, plaintext_len);

    if (ret == CMOX_CIPHER_AUTH_SUCCESS)
        return AES_SUCCESS;
    else if (ret == CMOX_CIPHER_AUTH_FAIL)
        return AES_AUTH_FAIL;
    else
        return AES_ERROR;
}

/* Private functions definition ----------------------------------------------*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
//#endif /* USERLIB_USE_ENABLED */ // plik.c
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
