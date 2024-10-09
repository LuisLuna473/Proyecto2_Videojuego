/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
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
#include "ili9341.h"
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
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
//extern uint8_t fondo[];
extern uint8_t Jug2 [];
extern uint8_t Jug1 [];
extern uint8_t colijug1 [];
extern uint8_t movjug1 [];
uint8_t data[];
int rect_x = 90, rect_y = 60;
int rect_width = 20, rect_height = 20;

/*int y_limit_min = 10, y_limit_max = 200;*/
int move_step = 10;
int jug_width = 32, jug_height = 31;

// Coordenadas iniciales del personaje Jug1
int jug_x = 150, jug_y = 200;

// Límite de movimiento en Y para Jug1
int y_limit_min = 120, y_limit_max = 240;

int green_rect_x = 150, green_rect_y = 60;
int green_rect_width = 20, green_rect_height = 20;

volatile uint8_t received_char = 0;  // Variable para almacenar el último carácter recibido
volatile uint8_t new_command = 0;  // Bandera para indicar si se recibió un nuevo comando

int colision = 0;

// Variable para mostrar en pantalla
char Pant[20];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
int check_collision(int jug_x, int jug_y, int green_x, int green_y) {
    return (jug_x < green_x + green_rect_width &&
            jug_x + jug_width > green_x &&
            jug_y < green_y + green_rect_height &&
            jug_y + jug_height > green_y);
}
void move_jug1_up() {
    // Limpiar la posición actual del personaje Jug1 (dibujar un rectángulo blanco sobre la posición anterior)
    FillRect(jug_x, jug_y, jug_width, jug_height, 0xFFFF);  // Color de fondo (blanco)

    // Mover hacia arriba si no ha alcanzado el límite
    if (jug_y - move_step >= y_limit_min) {
        jug_y -= move_step;
    }

    // Dibujar el personaje Jug1 en la nueva posición
    LCD_Bitmap(jug_x, jug_y, jug_width, jug_height, Jug1);  // Dibuja Jug1 en la nueva posición

    // Verificar colisión con el cubo verde
    if (check_collision(jug_x, jug_y, green_rect_x, green_rect_y)) {
    	colision++;
    }

    // Mostrar el contador de colisiones en pantalla
    sprintf(Pant, "Colisiones: %d", colision);
    LCD_Print(Pant, 20, 100, 1, 0x001F, 0xFFFF);  // Imprimir colisiones en la pantalla
}
void move_jug1_down() {
    // Limpiar la posición actual del personaje Jug1 (dibujar un rectángulo blanco sobre la posición anterior)
    FillRect(jug_x, jug_y, jug_width, jug_height, 0xFFFF);  // Color de fondo (blanco)

    // Mover hacia abajo si no ha alcanzado el límite
    if (jug_y + move_step <= y_limit_max) {
        jug_y += move_step;
    }

    // Dibujar el personaje Jug1 en la nueva posición
    LCD_Bitmap(jug_x, jug_y, jug_width, jug_height, Jug1);  // Dibuja Jug1 en la nueva posición

    // Verificar colisión con el cubo verde
    if (check_collision(jug_x, jug_y, green_rect_x, green_rect_y)) {
        colision++;
    }

    // Mostrar el contador de colisiones en pantalla
    sprintf(Pant, "Colisiones: %d", colision);
    LCD_Print(Pant, 20, 100, 1, 0x001F, 0xFFFF);  // Imprimir colisiones en la pantalla
}
/*void move_blue_cube_up() {
    // Limpiar el cubo azul actual (dibujar un rectángulo blanco sobre la posición anterior)
    FillRect(rect_x, rect_y, rect_width, rect_height, 0xFFFF);  // Color de fondo (blanco)

    // Mover hacia arriba si no ha alcanzado el límite
    if (rect_y - move_step >= y_limit_min) {
        rect_y -= move_step;
    }

    // Dibujar el cubo azul en la nueva posición
    FillRect(rect_x, rect_y, rect_width, rect_height, 0x001F);  // Azul

    // Verificar colisión con el cubo verde
    if (check_collision(rect_x, rect_y, green_rect_x, green_rect_y)) {
    	colision++;
    }

    // Mostrar el contador de colisiones en pantalla
    sprintf(Pant, "Colisiones: %d", colision);
    LCD_Print(Pant, 20, 100, 1, 0x001F, 0xFFFF);  // Imprimir colisiones en la pantalla
}

void move_blue_cube_down() {
    // Limpiar el cubo azul actual (dibujar un rectángulo blanco sobre la posición anterior)
    FillRect(rect_x, rect_y, rect_width, rect_height, 0xFFFF);  // Color de fondo (blanco)

    // Mover hacia abajo si no ha alcanzado el límite
    if (rect_y + move_step <= y_limit_max) {
        rect_y += move_step;
    }

    // Dibujar el cubo azul en la nueva posición
    FillRect(rect_x, rect_y, rect_width, rect_height, 0x001F);  // Azul

    // Verificar colisión con el cubo verde
    if (check_collision(rect_x, rect_y, green_rect_x, green_rect_y)) {
    	colision++;
    }

    // Mostrar el contador de colisiones en pantalla
    sprintf(Pant, "Colisiones: %d", colision);
    LCD_Print(Pant, 20, 100, 1, 0x001F, 0xFFFF);  // Imprimir colisiones en la pantalla
}*/

void move_green_cube_left() {
    // Limpiar el cubo verde actual
    FillRect(green_rect_x, green_rect_y, green_rect_width, green_rect_height, 0xFFFF);  // Color de fondo (blanco)

    // Mover el cubo verde hacia la izquierda
    green_rect_x -= move_step;

    // Dibujar el cubo verde en la nueva posición
    FillRect(green_rect_x, green_rect_y, green_rect_width, green_rect_height, 0x07E0);  // Verde
}

void move_green_cube_right() {
    // Limpiar el cubo verde actual
    FillRect(green_rect_x, green_rect_y, green_rect_width, green_rect_height, 0xFFFF);  // Color de fondo (blanco)

    // Mover el cubo verde hacia la derecha
    green_rect_x += move_step;

    // Dibujar el cubo verde en la nueva posición
    FillRect(green_rect_x, green_rect_y, green_rect_width, green_rect_height, 0x07E0);  // Verde
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
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  /* USER CODE BEGIN 2 */
    HAL_UART_Receive_IT(&huart3, data, 1);
	LCD_Init();

	LCD_Clear(0x00);
	FillRect(0, 0, 319, 239, 0xFFFF);
	FillRect(50, 60, 20, 20, 0xF800);
	FillRect(70, 60, 20, 20, 0x07E0);
	//LCD_Bitmap(0, 0, 320, 240, fondo);
	//LCD_Bitmap(0, 0, 32, 31, Jug2);
	LCD_Bitmap(50, 150, 32, 31, Jug1);
	FillRect(90, 60, 20, 20, 0x001F);


	//FillRect(0, 0, 319, 206, 0x1911);

	 //LCD_Print("Hola Mundo", 20, 100, 1, 0x001F, 0xCAB9);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		/*for(int x = 0; x < 320 -32; x++){
			HAL_Delay(15);
			int anima = (x/35)%3;
			LCD_Sprite(0, 200, 31, 32, colijug1, 3, anima, 0, 1);
		}*/
		for(int c = 0; c < 320 -32; c++){
			HAL_Delay(15);
			int anima1 = (c/2)%2;
			LCD_Sprite(0, 200, 29, 41, movjug1, 2, anima1, 0, 1);
		}

		if (received_char == 'g') {
		    //move_blue_cube_up();  // Mover cubo azul hacia arriba
		    move_jug1_up();
		    received_char = 0;  // Resetear el comando
		}


		if (received_char == 'h') {
		    //move_blue_cube_down();  // Mover cubo azul hacia abajo
			move_jug1_down();
		    received_char = 0;  // Resetear el comando
		}
		if (received_char == 'e') {
		    move_green_cube_left();  // Mover cubo verde hacia la izquierda
		    received_char = 0;  // Resetear el comando
		}
		if(received_char == 'f'){
			move_green_cube_right();
			received_char = 0;
		}
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 80;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 9600;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|LCD_D1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin|SD_SS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_RST_Pin LCD_D1_Pin */
  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_RD_Pin LCD_WR_Pin LCD_RS_Pin LCD_D7_Pin
                           LCD_D0_Pin LCD_D2_Pin */
  GPIO_InitStruct.Pin = LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin
                          |LCD_D0_Pin|LCD_D2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_CS_Pin LCD_D6_Pin LCD_D3_Pin LCD_D5_Pin
                           LCD_D4_Pin SD_SS_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin|SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	received_char = data[0];
	new_command = 1;
	HAL_UART_Receive_IT(&huart3, data, 1);

}
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
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
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
