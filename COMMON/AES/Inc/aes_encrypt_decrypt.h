/*
 * aes_encrypt_decrypt.h
 *
 *  Created on: Oct 18, 2025
 *      Author: adminn
 */


#ifndef COMMON_AES_INC_AES_ENCRYPT_DECRYPT_H_
#define COMMON_AES_INC_AES_ENCRYPT_DECRYPT_H_

#ifdef __cplusplus // plik.h
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/

#include <stddef.h>
#include <stdint.h>

/* Public define -------------------------------------------------------------*/
#define AES_KEY_SIZE_256     32U
#define AES_IV_SIZE          12U
#define AES_TAG_SIZE         16U
/* Public macros -------------------------------------------------------------*/

/* Public typedef ------------------------------------------------------------*/
typedef enum {
    AES_SUCCESS = 0,
    AES_AUTH_FAIL,
    AES_ERROR
} aes_status_t;
/* Public constants ----------------------------------------------------------*/

/* Public variables ----------------------------------------------------------*/

/* Public function declaration -----------------------------------------------*/

aes_status_t AES_Crypto_Init(void);
aes_status_t AES_Crypto_Finalize(void);

aes_status_t AES_GCM_Encrypt(
    const uint8_t *plaintext, size_t plaintext_len,
    const uint8_t *key, size_t key_len,
    const uint8_t *iv, size_t iv_len,
    const uint8_t *aad, size_t aad_len,
    uint8_t *ciphertext_and_tag, size_t *output_len);

aes_status_t AES_GCM_Decrypt(
    const uint8_t *ciphertext_and_tag, size_t input_len,
    const uint8_t *key, size_t key_len,
    const uint8_t *iv, size_t iv_len,
    const uint8_t *aad, size_t aad_len,
    uint8_t *plaintext, size_t *plaintext_len);

#ifdef __cplusplus // plik.h
}
#endif

#endif /* COMMON_AES_INC_AES_ENCRYPT_DECRYPT_H_ */
