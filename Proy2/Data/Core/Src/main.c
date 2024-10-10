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
#include "fatfs.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ili9341.h"
#include "fatfs_sd.h"
#include "string.h"
#include "stdio.h"

#include <stdlib.h>  // Para rand() y srand()
#include <time.h>    // Para time()
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BUF_LEN 2048
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_rx;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
//extern uint8_t FONDOINICIO[];
extern DIVISIONBAJA [];
extern DIVISION [];
extern TRACK [];

extern LOGOEX [];
extern OPCION1[];
extern OPCION2 [];
extern OPCION3 [];
extern PUNTERO [];

extern uint8_t Jug2 [];
extern uint8_t Jug1 [];
extern uint8_t colijug1 [];
extern uint8_t movjug1 [];
extern uint8_t CACTUS[];

uint8_t data[];
int inicio = 3;
int loser1 = 0;
int loser2 = 0;
int win = 0;
int pintjueg = 0; //Pintar una ves pantalla juego
uint8_t opcion = 5;

int rect_x = 90, rect_y = 60;
int rect_width = 20, rect_height = 20;

/*int y_limit_min = 10, y_limit_max = 200;*/
int move_step = 10;
int jug_width = 32, jug_height = 31;

// Coordenadas iniciales del personaje Jug1
int jug_x = 150, jug_y = 200;

// Límite de movimiento en Y para Jug1
int y_limit_min = 140, y_limit_max = 210;

int green_rect_x = 150, green_rect_y = 150;
int green_rect_width = 20, green_rect_height = 20;

int z = 300;
uint32_t last_time_cactus = 0;

volatile uint8_t received_char = 0;  // Variable para almacenar el último carácter recibido
volatile uint8_t new_command = 0;  // Bandera para indicar si se recibió un nuevo comando

int colision = 0;

int cactus_pos = 300;  // Posición inicial del cactus
int cactus_state = 0;  // 0: inactivo, 1: activo y moviéndose
int cactus_timer = 0;  // Temporizador simple para el retardo entre pasos

int c = 0; // Para manejar el contador horizontal
int y_pos = 200; // Posición inicial del personaje en el eje Y
int anima1 = 0; // Para controlar el índice de animación

int cactus = 0;
// Límites para el movimiento vertical Jug1


const int sprite_width = 30;  // Ancho del sprite
const int sprite_height = 29; // Altura del sprite
int sprite_x = 50;
int sprite_y = 150;

int move1 = 0; //Bandera de movimiento sprite
// Variable para mostrar en pantalla
char Pant[20];

// Definir coordenadas Y para las opciones
int OPCION1_Y = 130;
int OPCION2_Y = 162;
int OPCION3_Y = 195;

// Definir tamaño y posición del puntero
int PUNTERO_X = 80;
int PUNTERO_ANCHO = 14;
int PUNTERO_ALTO = 15;

uint8_t seleccion_actual = 1;

//SD control
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char buffer[4500];
unsigned char buffer[4500];
uint8_t leer;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void dibujar_puntero(uint8_t y_coord) {
    // Esta función dibuja el puntero en la posición `y_coord` en la pantalla.
    LCD_Bitmap(80, y_coord, 14, 15, PUNTERO);  // Dibuja el puntero en la nueva coordenada Y
}

void borrar_puntero(uint8_t y_coord) {
    // Esta función borra el puntero en la posición `y_coord` de la pantalla.
    // Aquí deberías usar un bitmap vacío o redibujar la parte de la pantalla sin el puntero.
    FillRect(80, y_coord, 14, 15, 0x2817);  // Borra el puntero en la coordenada Y
}

// Función para mover el puntero hacia arriba
void mover_puntero_arriba(void) {
    if (seleccion_actual > 1) {
        // Borrar puntero en la posición actual
        borrar_puntero(seleccion_actual == 2 ? OPCION2_Y : OPCION3_Y);

        // Cambiar a la opción anterior
        seleccion_actual--;

        // Dibujar el puntero en la nueva posición
        dibujar_puntero(seleccion_actual == 1 ? OPCION1_Y : OPCION2_Y);
    }
}

// Función para mover el puntero hacia abajo
void mover_puntero_abajo(void) {
    if (seleccion_actual < 3) {
        // Borrar puntero en la posición actual
        borrar_puntero(seleccion_actual == 1 ? OPCION1_Y : OPCION2_Y);

        // Cambiar a la siguiente opción
        seleccion_actual++;

        // Dibujar el puntero en la nueva posición
        dibujar_puntero(seleccion_actual == 2 ? OPCION2_Y : OPCION3_Y);
    }
}


int check_collision(int jug_x, int jug_y, int green_x, int green_y) {
    return (jug_x < green_x + green_rect_width &&
            jug_x + jug_width > green_x &&
            jug_y < green_y + green_rect_height &&
            jug_y + jug_height > green_y);
}

void move_green_cube_left() {
    // Limpiar el cubo verde actual
    FillRect(green_rect_x, green_rect_y, green_rect_width, green_rect_height, 0xEDCC);  // Color de fondo (blanco)

    // Mover el cubo verde hacia la izquierda
    green_rect_x -= move_step;

    // Dibujar el cubo verde en la nueva posición
    FillRect(green_rect_x, green_rect_y, green_rect_width, green_rect_height, 0x07E0);  // Verde
}

void move_green_cube_right() {
    // Limpiar el cubo verde actual
    FillRect(green_rect_x, green_rect_y, green_rect_width, green_rect_height, 0xEDCC);  // Color de fondo (blanco)

    // Mover el cubo verde hacia la derecha
    green_rect_x += move_step;

    // Dibujar el cubo verde en la nueva posición
    FillRect(green_rect_x, green_rect_y, green_rect_width, green_rect_height, 0x07E0);  // Verde
}

void clear_previous_sprite(int x, int y, int width, int height) {
    // Limpia la posición anterior del sprite dibujando un rectángulo blanco
    FillRect(x, y, width, height, 0xEDCC);  // Color blanco (0xFFFF) para limpiar
}

void move_sprite_up() {
    // Limpiar la posición actual del sprite antes de moverlo
    clear_previous_sprite(sprite_x, sprite_y, sprite_width, sprite_height);

    // Verificar si no se sale del límite superior antes de mover
    if (sprite_y - move_step >= y_limit_min) {
        sprite_y -= move_step;  // Mover hacia arriba
    }

    // Redibujar el sprite en la nueva posición
    //LCD_Sprite(sprite_x, sprite_y, sprite_width, sprite_height, movjug1, 2, anima1, 0, 1);  // Dibuja el sprite en su nueva posición
    LCD_Bitmap(sprite_x, sprite_y, sprite_width, sprite_height, Jug1);
    if (check_collision(sprite_x, sprite_y, z, 150)) { //check_collision(sprite_x, sprite_y, green_rect_x, green_rect_y
        	colision++;
    }

    // Mostrar el contador de colisiones en pantalla
    /*sprintf(Pant, "Colisiones: %d", colision);
    LCD_Print(Pant, 20, 100, 1, 0x001F, 0xEDCC);*/  // Imprimir colisiones en la pantalla

}

void move_sprite_down() {
    // Limpiar la posición actual del sprite antes de moverlo
    clear_previous_sprite(sprite_x, sprite_y, sprite_width, sprite_height);

    // Verificar si no se sale del límite inferior antes de mover
    if (sprite_y + move_step + sprite_height <= y_limit_max) {
        sprite_y += move_step;  // Mover hacia abajo
    }

    // Redibujar el sprite en la nueva posición
    //LCD_Sprite(sprite_x, sprite_y, sprite_width, sprite_height, movjug1, 2, anima1, 0, 1);  // Dibuja el sprite en su nueva posición

    LCD_Bitmap(sprite_x, sprite_y, sprite_width, sprite_height, Jug1);
    if (check_collision(sprite_x, sprite_y, z, 200)) {
    	colision++;
    }

    // Mostrar el contador de colisiones en pantalla
    /*sprintf(Pant, "Colisiones: %d", colision);
    LCD_Print(Pant, 20, 100, 1, 0x001F, 0xEDCC);  // Imprimir colisiones en la pantalla*/
}


/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	srand(time(NULL));
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
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_USART3_UART_Init();
  MX_FATFS_Init();
  /* USER CODE BEGIN 2 */
    HAL_UART_Receive_IT(&huart3, data, 1);
	LCD_Init();

	LCD_Clear(0x00);

	/*FillRect(0, 0, 319, 239, 0xFFFF);
	FillRect(50, 60, 20, 20, 0xF800);
	FillRect(70, 60, 20, 20, 0x07E0);*/
	//LCD_Bitmap(0, 0, 320, 240, FONDOINICIO);

	LCD_Clear(0x2817);
	LCD_Bitmap(53, 25, 214, 85, LOGOEX);
	LCD_Bitmap(110, 130, 98, 12, OPCION1);
	LCD_Bitmap(110, 162, 142, 13, OPCION2);
	LCD_Bitmap(110, 195, 84, 11, OPCION3);
	LCD_Bitmap(80, 130, 14, 15, PUNTERO);

	//LCD_Sprite(sprite_x, sprite_y, sprite_width, sprite_height, movjug1, 2, anima1, 0, 1);

	//FillRect(0, 0, 319, 206, 0x1911);

	 //LCD_Print("Hola Mundo", 20, 100, 1, 0x001F, 0xCAB9);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		if(inicio == 3 && received_char != new_command){ //
			//HAL_Delay(200);
			if(received_char == 'd'){
				mover_puntero_arriba();
			}
			else if(received_char == 'c'){
				mover_puntero_abajo();
			}

			new_command == received_char;
		}

		if(received_char == 'a' && seleccion_actual == 1){
			inicio = 0;
			seleccion_actual = 0;
			pintjueg = 1;
		}

		switch (inicio){
			case 0:

				if(pintjueg == 1){
					LCD_Clear(0xEDCC);
					/*fres = f_mount(&fs, "/", 0);
					if(fres == FR_OK){
						LCD_Print("Micro SD card is mounted succesfully!", 20, 50, 1, 0x001F, 0xCAB9);
					}else if(fres != FR_OK){
						LCD_Print("Micro SD card's mount error!", 20, 100, 1, 0x001F, 0xCAB9);
					}
					fres = f_open(&fil, "DIVISION.txt", FA_READ);
					if(fres == FR_OK){
						LCD_Print("File opened for reading.", 20, 100, 1, 0x001F, 0xCAB9);
						leer = 1;
					}else if(fres != FR_OK){
						LCD_Print("File was not opened for reading!", 30, 100, 1, 0x001F, 0xCAB9);
						leer = 0;
					}
					if (leer == 1){
						while(f_gets(buffer, sizeof(buffer), &fil)){
						}
						leer = 0;
					}
					fres = f_close(&fil);
					if(fres == FR_OK){
						LCD_Print("The file is closed", 30, 150, 1, 0x001F, 0xCAB9);
					}else if(fres != FR_OK){
						LCD_Print("The file was not closed", 30, 150, 1, 0x001F, 0xCAB9);
					}
					f_mount(NULL,"",1);

					LCD_Bitmap(0, 120, 120,19, buffer);
					LCD_Bitmap(125, 120, 120,19, buffer);
					LCD_Bitmap(200, 120, 120,19, buffer);
					memset(buffer, '\0', 4500); //Limpia el buffer*/

					//División de carriles
					LCD_Bitmap(0, 120, 120,19, DIVISION);
					LCD_Bitmap(125, 120, 120,19, DIVISION);
					LCD_Bitmap(200, 120, 120,19, DIVISION);


					LCD_Bitmap(0, 220, 80,18, DIVISIONBAJA);
					LCD_Bitmap(80, 220, 80,18, DIVISIONBAJA);
					LCD_Bitmap(160, 220, 80,18, DIVISIONBAJA);
					LCD_Bitmap(240, 220, 80,18, DIVISIONBAJA);
					//Público
					LCD_Bitmap(0, 0,320, 45 , TRACK);

					LCD_Bitmap(0, 0, 32, 31, Jug1);
					FillRect(33, 0, 20, 20, 0xF800); //Vidas Jug1
					FillRect(53, 0, 20, 20, 0xF800);
					FillRect(73, 0, 20, 20, 0xF800);
					FillRect(93, 0, 20, 20, 0xF800);

					LCD_Bitmap(0, 210, 32, 31, Jug2);
					FillRect(33, 220, 20, 20, 0x001F); //Vidas Jug2
					FillRect(53, 220, 20, 20, 0x001F);
					FillRect(73, 220, 20, 20, 0x001F);
					FillRect(93, 220, 20, 20, 0x001F);
					pintjueg = 0;
				}
				uint32_t current_time = HAL_GetTick();
				int random_number = (rand() % 5) + 1;//Numeros random del 1 al 5
				/*for(int x = 0; x < 320 -32; x++){
					HAL_Delay(15);
					int anima = (x/35)%3;
					LCD_Sprite(0, 200, 31, 32, colijug1, 3, anima, 0, 1);
				}*/
				/*for(int c = 0; c < 320 -32; c++){
					HAL_Delay(15);
					int anima1 = (c/2)%2;
					LCD_Sprite(0, 200, 29, 41, movjug1, 2, anima1, 0, 1);
				}*/
				if (random_number == 1  && cactus != 1) {//&& move1 != 1
					if (current_time - last_time_cactus >= 15) { // Controla la velocidad del cactus
						last_time_cactus = current_time;
						clear_previous_sprite(z, 150, 20, 20);
						// Mover cactus hacia la izquierda
						z--; // Decrementa la posición del cactus
						int anima = (z / 50) % 1;
						//LCD_Sprite(z, 150, 20, 25, CACTUS, 1, anima1, 0, 1);
						FillRect(z, 150, 20, 20, 0xF800);
						if (check_collision(sprite_x, sprite_y, z, 150)) {
							colision++;
							// Borrar el cactus cuando hay colisión
						    FillRect(z, 150, 20, 25, 0xEDCC); // Rellenar con color de fondo
						    cactus = 1;
						}
						// Si el cactus llega al límite, reiniciar su posición
						if (z <= 10) {
							z = 300;  // Reiniciar la posición del cactus
						}
						// Si el cactus llega al límite, reiniciar su posición
						if (z <= 10) {
							FillRect(z, 150, 20, 25, 0xEDCC); // Reiniciar la posición del cactus
						    z = 300;
						}
					}
				}
				if(colision == 1){
					FillRect(93, 0, 20, 20, 0xFFFF);
				}else if(colision == 2){
					FillRect(73, 0, 20, 20, 0xFFFF);
				}else if(colision == 3){
					FillRect(53, 0, 20, 20, 0xFFFF);
				}else if(colision == 4){
					FillRect(33, 0, 20, 20, 0xFFFF);
					loser1 = 1;
				}
				if(loser1 == 1){
					LCD_Print("Perdiste! JUG1 :(", 20, 100, 1, 0x001F, 0xCAB9);
				}
				if (received_char == 'g') {
					//move_blue_cube_up();  // Mover cubo azul hacia arriba
					//move_jug1_up();
					move_sprite_up();
					received_char = 0;  // Resetear el comando
					move1 = 0;
				}
				if (received_char == 'h') {
					//move_blue_cube_down();  // Mover cubo azul hacia abajo
					//move_jug1_down();
					move_sprite_down();
					received_char = 0;  // Resetear el comando
					move1 = 0;
				}
				if (received_char == 'e') {
					//move_green_cube_left();  // Mover cubo verde hacia la izquierda
					received_char = 0;  // Resetear el comando
					move1 = 0;
				}
				if(received_char == 'f'){
					//move_green_cube_right();
					received_char = 0;
					move1 = 0;
				}
				/*if(move1 != 1){
					c++;
					anima1 = (c / 2) % 2;
					LCD_Sprite(sprite_x, sprite_y, sprite_width, sprite_height, movjug1, 2, anima1, 0, 1);
				}
				if(c == 20){
					c = 0;
				}*/
				break;
			case 1:

				break;
			case 2:
				break;
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
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);

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
                          |LCD_D4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SD_SS_GPIO_Port, SD_SS_Pin, GPIO_PIN_SET);

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
                           LCD_D4_Pin */
  GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin
                          |LCD_D4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : SD_SS_Pin */
  GPIO_InitStruct.Pin = SD_SS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(SD_SS_GPIO_Port, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	received_char = data[0];
	move1 = 1;
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
