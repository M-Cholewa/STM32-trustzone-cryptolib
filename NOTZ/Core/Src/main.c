/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "cmox_crypto.h"
#include "aes_encrypt_decrypt.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

CRC_HandleTypeDef hcrc;

DCACHE_HandleTypeDef hdcache1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_ICACHE_Init(void);
static void MX_DCACHE1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define NUM_TEST_SIZES 10

static const size_t test_sizes[NUM_TEST_SIZES] = {
    4*1024,    // 4 KB
    8*1024,    // 8 KB
    16*1024,   // 16 KB
    24*1024,   // 24 KB
    32*1024,   // 32 KB
    48*1024,   // 48 KB
    64*1024,   // 64 KB
    96*1024,   // 96 KB
    128*1024,  // 128 KB
    192*1024   // 192 KB
};
/* Klucz / IV / AAD testowe */
static const uint8_t AES_Key[AES_KEY_SIZE_256] = {
  0x46,0x3b,0x41,0x29,0x11,0x76,0x7d,0x57,
  0xa0,0xb3,0x39,0x69,0xe6,0x74,0xff,0xe7,
  0x84,0x5d,0x31,0x3b,0x88,0xc6,0xfe,0x31,
  0x2f,0x3d,0x72,0x4b,0xe6,0x8e,0x1f,0xca
};

static const uint8_t AES_IV[AES_IV_SIZE] = {
  0x61,0x1c,0xe6,0xf9,0xa6,0x88,0x07,0x50,0xde,0x7d,0xa6,0xcb
};

static const uint8_t AES_AAD[] = { 0x01, 0x02, 0x03, 0x04 };

typedef struct {
    size_t data_size;
    uint32_t enc_time_ms;
    uint32_t dec_time_ms;
    aes_status_t enc_status;
    aes_status_t dec_status;
    uint8_t verified;
} aes_test_result_t;

static aes_test_result_t aes_results[NUM_TEST_SIZES];

/* ============================ Pomocnicze ============================ */

static void FillBuffer(uint8_t *buf, size_t len)
{
    for (size_t i = 0; i < len; i++)
        buf[i] = (uint8_t)(i & 0xFF);
}

#define MAX_TEST_SIZE (192*1024)  // 192 KB

static uint8_t plaintext[MAX_TEST_SIZE];
static uint8_t ciphertext[MAX_TEST_SIZE + AES_TAG_SIZE];
static uint8_t decrypted[MAX_TEST_SIZE];

/* ============================ Funkcja testująca ============================ */
static void Run_AESEncryptDecrypt_Test(size_t index)
{
    size_t data_size = test_sizes[index];
    size_t enc_len = 0, dec_len = 0;
    uint32_t start, stop;
    aes_status_t status;

    aes_results[index].data_size = data_size;

    FillBuffer(plaintext, data_size);

    /* ============ ENCRYPT ============ */
    start = HAL_GetTick();

#ifdef TRUSTZONE_NONSECURE
    status = SECURE_AES_GCM_Encrypt(
                plaintext, data_size,
                ciphertext, &enc_len);
#else
    status = AES_GCM_Encrypt(
                plaintext, data_size,
                AES_Key, sizeof(AES_Key),
                AES_IV, sizeof(AES_IV),
                AES_AAD, sizeof(AES_AAD),
                ciphertext, &enc_len);
#endif

    stop = HAL_GetTick();
    aes_results[index].enc_time_ms = stop - start;
    aes_results[index].enc_status = status;

    if (status != AES_SUCCESS)
        Error_Handler();

    /* ============ DECRYPT ============ */
    start = HAL_GetTick();

#ifdef TRUSTZONE_NONSECURE
    status = SECURE_AES_GCM_Decrypt(
                ciphertext, enc_len,
                decrypted, &dec_len);
#else
    status = AES_GCM_Decrypt(
                ciphertext, enc_len,
                AES_Key, sizeof(AES_Key),
                AES_IV, sizeof(AES_IV),
                AES_AAD, sizeof(AES_AAD),
                decrypted, &dec_len);
#endif

    stop = HAL_GetTick();
    aes_results[index].dec_time_ms = stop - start;
    aes_results[index].dec_status = status;

    if (status != AES_SUCCESS)
        Error_Handler();

    /* ============ Verify ============ */
    if ((dec_len == data_size) && (memcmp(plaintext, decrypted, data_size) == 0))
        aes_results[index].verified = 1;
    else
        Error_Handler();
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
//  MX_CRC_Init();
//  MX_ICACHE_Init();
//  MX_DCACHE1_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  /* Init crypto lib */
  if (AES_Crypto_Init() != AES_SUCCESS)
      Error_Handler();

  for (size_t i = 0; i < NUM_TEST_SIZES; i++)
      Run_AESEncryptDecrypt_Test(i);

  AES_Crypto_Finalize();


  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV2;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_PCLK3;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the programming delay
  */
  __HAL_FLASH_SET_PROGRAM_DELAY(FLASH_PROGRAMMING_DELAY_0);
}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DCACHE1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_DCACHE1_Init(void)
{

  /* USER CODE BEGIN DCACHE1_Init 0 */

  /* USER CODE END DCACHE1_Init 0 */

  /* USER CODE BEGIN DCACHE1_Init 1 */

  /* USER CODE END DCACHE1_Init 1 */
  hdcache1.Instance = DCACHE1;
  hdcache1.Init.ReadBurstType = DCACHE_READ_BURST_WRAP;
  if (HAL_DCACHE_Init(&hdcache1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DCACHE1_Init 2 */

  /* USER CODE END DCACHE1_Init 2 */

}

/**
  * @brief ICACHE Initialization Function
  * @param None
  * @retval None
  */
static void MX_ICACHE_Init(void)
{

  /* USER CODE BEGIN ICACHE_Init 0 */

  /* USER CODE END ICACHE_Init 0 */

  /* USER CODE BEGIN ICACHE_Init 1 */

  /* USER CODE END ICACHE_Init 1 */

  /** Enable instruction cache (default 2-ways set associative cache)
  */
  if (HAL_ICACHE_Enable() != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ICACHE_Init 2 */

  /* USER CODE END ICACHE_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
