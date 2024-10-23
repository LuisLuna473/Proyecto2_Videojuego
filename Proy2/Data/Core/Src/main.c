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

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart3_rx;

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
extern uint8_t movjug1 [];
extern uint8_t movjug2 [];
extern uint8_t movjug3 [];
extern uint8_t movjug4 [];
extern uint8_t colisionjug1 [];
extern uint8_t colisionjug2 [];
extern uint8_t colisionjug3 [];
extern uint8_t colisionjug4 [];
extern uint8_t derrapejug1 [];
extern uint8_t derrapejug2 [];
extern uint8_t derrapejug3 [];
extern uint8_t derrapejug4 [];
extern uint8_t SELECTOROFF [];
extern uint8_t SELECTORONJ1 [];
extern uint8_t SELECTORONJ2 [];
extern uint8_t SELECTOROFFJ1[];
extern uint8_t personaje1 [];
extern uint8_t personaje2 [];
extern uint8_t personaje3 [];
extern uint8_t personaje4 [];
extern uint8_t AVISOJUGADOR [];

extern uint8_t exciteinicio [];
extern uint8_t DESERT [];
extern uint8_t LOGONINTENDO [];

extern uint8_t ENEMY[];
extern uint8_t BOOSTER[];

extern uint8_t ICONJ1[];
extern uint8_t LETRAJ1[];
extern uint8_t ICONJ2[];
extern uint8_t LETRAJ2[];
extern uint8_t MURO[];
extern uint8_t GUSANOJ1[];
extern uint8_t GUSANOJ2[];
extern uint8_t CAMEL[];

extern uint8_t POKEY [];
extern uint8_t giroPOKEY [];

//Ganadores
extern uint8_t podio[];
extern uint8_t trofeo[];
extern uint8_t aviso1[];
extern uint8_t aviso2[];


uint8_t musica[0]; //Buffer para la musica vía DMA envio
uint8_t data[];
uint8_t data2[];

int inicio = 5;
int start = 0;
int unico = 1;

int loser1 = 0;
int loser2 = 0;
int win = 0;
int pintjueg = 0; //Pintar una ves pantalla juego
uint8_t opcion = 5;

int bandera = 1;

//int rect_x = 90, rect_y = 60;
int rect_width = 20, rect_height = 20;

/*int y_limit_min = 10, y_limit_max = 200;*/
int move_step = 10;
int move_step1 = 10;


int jug_width = 32, jug_height = 31;

// Coordenadas iniciales del personaje Jug1
int jug_x = 150, jug_y = 200;

// Límite de movimiento en Y para Jug2
int y_limit_min = 140, y_limit_max = 210;

//Límite de movimiento en Y para Jug1
int y1_limit_min = 40, y1_limit_max = 100;

int green_rect_x = 150, green_rect_y = 150;
int green_rect_width = 20, green_rect_height = 20;

//Variables de movimiento ENEMY en X
int z = 300;
int u = 300;


int selector1_pos = 1; // Posición del selector 1 (Jugador 1), valores entre 1 y 4.
int selector2_pos = 4; // Posición del selector 2 (Jugador 2), valores entre 1 y 4.
uint8_t* seleccionJug1 = movjug1;
uint8_t* seleccionJug2 = movjug2;
uint8_t* caida1 = colisionjug1;
uint8_t* caida2 = colisionjug2;

uint32_t last_time_cactus = 0;

volatile uint8_t received_char = 0;  // Variable para almacenar el último carácter recibido
volatile uint8_t received_char2 = 0;
volatile uint8_t new_command = 0;  // Bandera para indicar si se recibió un nuevo comando

int colision = 0;
int colision2 = 0;

int cactus_pos = 300;  // Posición inicial del cactus
int cactus_state = 0;  // 0: inactivo, 1: activo y moviéndose
int cactus_timer = 0;  // Temporizador simple para el retardo entre pasos

int c = 0; // Para manejar el contador horizontal
int m = 0;

int random_number;

//Selectores para mountruos
int selector1 = 0;
int selector2 = 0;

int tiempo = 0;
int tiempo1 = 0;
int tiempo2 = 0;
int delay_i;
int last_time_i = 50;

int y_pos = 200; // Posición inicial del personaje en el eje Y
int anima1 = 0; // Para controlar el índice de animación
int anima2 = 0;

int cactus = 0;

int boom1 = 0;
int boom2 = 0;


const int sprite_width = 34;  // Ancho del sprite
const int sprite_height = 37; // Altura del sprite
int sprite_x = 50;
int sprite_y = 150;

const int sprite_width1 = 34;
const int sprite_height1 = 37;

int sprite_x1 = 50;
int sprite_y1 = 50;

int move1 = 0; //Bandera de movimiento sprite
int move2 = 0;
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

uint8_t seleccion_actual = 0;

//SD control
FATFS fs;
FATFS *pfs;
FIL fil;
FRESULT fres;
DWORD fre_clust;
uint32_t totalSpace, freeSpace;
char buffer[16000]; //Buffer para leer 10640 caracteres de personaje1.txt
uint8_t leer;

#define IMAGE_SIZE 16000 // Ajusta el tamaño

uint8_t image[IMAGE_SIZE];  // Variable para almacenar los datos de la imagen
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_UART5_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void transmit_uart(char *string);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

int load_image_from_sd_to_play(const char* filename) {
    FIL fil;
    UINT bytes_read;
    FRESULT fres;

    // Abrir el archivo desde la SD
    fres = f_open(&fil, filename, FA_READ);
    if (fres != FR_OK) {
      //  transmit_uart("Error al abrir el archivo en la SD\n");
        return 0;  // Error al abrir el archivo
    }

    // Leer los datos del archivo y almacenarlos en la variable play
    fres = f_read(&fil, image, IMAGE_SIZE, &bytes_read);
    if (fres != FR_OK || bytes_read == 0) {
      //  transmit_uart("Error al leer la imagen desde la SD\n");
        f_close(&fil);
        return 0;  // Error al leer el archivo
    }

    // Cerrar el archivo
    f_close(&fil);
   // transmit_uart("Imagen cargada correctamente desde la SD a la variable play\n");
    return 1;  // Éxito
}


// Función para actualizar los selectores en la pantalla.
void actualizar_selectores() {
    // Limpiar la pantalla
    LCD_Bitmap(25, 70, 63, 17, SELECTOROFFJ1);
    LCD_Bitmap(95, 70, 63, 17, SELECTOROFFJ1);
    LCD_Bitmap(165, 70, 63, 17, SELECTOROFFJ1);
    LCD_Bitmap(235, 70, 63, 17, SELECTOROFFJ1);

    LCD_Bitmap(25, 131, 63, 17, SELECTOROFFJ1);
    LCD_Bitmap(95, 131, 63, 17, SELECTOROFFJ1);
    LCD_Bitmap(165, 131, 63, 17, SELECTOROFFJ1);
    LCD_Bitmap(235, 131, 63, 17, SELECTOROFFJ1);

    // Dibujar selector del Jugador 1
    switch (selector1_pos) {
        case 1:
            LCD_Bitmap(25, 70, 63, 17, SELECTORONJ1);
            break;
        case 2:
            LCD_Bitmap(95, 70, 63, 17, SELECTORONJ1);
            break;
        case 3:
            LCD_Bitmap(165, 70, 63, 17, SELECTORONJ1);
            break;
        case 4:
            LCD_Bitmap(235, 70, 63, 17, SELECTORONJ1);
            break;
    }

    // Dibujar selector del Jugador 2
    switch (selector2_pos) {
        case 1:
            LCD_Bitmap(25, 131, 63, 17, SELECTORONJ2);
            break;
        case 2:
            LCD_Bitmap(95, 131, 63, 17, SELECTORONJ2);
            break;
        case 3:
            LCD_Bitmap(165, 131, 63, 17, SELECTORONJ2);
            break;
        case 4:
            LCD_Bitmap(235, 131, 63, 17, SELECTORONJ2);
            break;
    }
}

// Función para manejar los comandos de entrada
void mover_colores(char comando) {
    switch (comando) {
        case 'a': // Mover selector 1 a la izquierda
            if (selector1_pos > 1) {
                selector1_pos--;
            }
            break;
        case 'b': // Mover selector 1 a la derecha
            if (selector1_pos < 4) {
                selector1_pos++;
            }
            break;
        case 'f': // Mover selector 2 a la izquierda
            if (selector2_pos > 1) {
                selector2_pos--;
            }
            break;
        case 'e': // Mover selector 2 a la derecha
            if (selector2_pos < 4) {
                selector2_pos++;
            }
            break;
    }

    // Actualizar la pantalla después de mover el selector
    actualizar_selectores();
}

void dibujar_puntero(uint8_t y_coord) {
    // Dibuja el puntero en la posición `y_coord` en la pantalla.
    LCD_Bitmap(80, y_coord, 14, 15, PUNTERO);
}

void borrar_puntero(uint8_t y_coord) {
    // Borra el puntero en la posición `y_coord` de la pantalla.
    FillRect(80, y_coord, 14, 15, 0x2817);
}

// Función para mover el puntero hacia arriba
void mover_puntero_arriba(void) {
    if (seleccion_actual == 2) {
        // Borrar el puntero en la posición actual (segunda opción)
        borrar_puntero(OPCION2_Y);

        // Cambiar a la primera opción
        seleccion_actual = 1;

        // Dibujar el puntero en la nueva posición (primera opción)
        dibujar_puntero(OPCION1_Y);
    }
}

// Función para mover el puntero hacia abajo
void mover_puntero_abajo(void) {
    if (seleccion_actual == 1) {
        // Borrar el puntero en la posición actual (primera opción)
        borrar_puntero(OPCION1_Y);

        // Cambiar a la segunda opción
        seleccion_actual = 2;

        // Dibujar el puntero en la nueva posición (segunda opción)
        dibujar_puntero(OPCION2_Y);
    }
}


int check_collision(int jug_x, int jug_y, int green_x, int green_y) {
    return (jug_x < green_x + green_rect_width &&
            jug_x + jug_width > green_x &&
            jug_y < green_y + green_rect_height &&
            jug_y + jug_height > green_y);
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
    LCD_Sprite(sprite_x, sprite_y, sprite_width, sprite_height, seleccionJug1, 2, anima1, 0, 1);  // Dibuja el sprite en su nueva posición
    //LCD_Bitmap(sprite_x, sprite_y, sprite_width, sprite_height, Jug1);
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
    LCD_Sprite(sprite_x, sprite_y, sprite_width, sprite_height, seleccionJug1, 2, anima1, 0, 1);  // Dibuja el sprite en su nueva posición

    //LCD_Bitmap(sprite_x, sprite_y, sprite_width, sprite_height, Jug1);
    if (check_collision(sprite_x, sprite_y, z, 200)) {
    	colision++;
    }

    // Mostrar el contador de colisiones en pantalla
    /*sprintf(Pant, "Colisiones: %d", colision);
    LCD_Print(Pant, 20, 100, 1, 0x001F, 0xEDCC);  // Imprimir colisiones en la pantalla*/
}

void move_sprite_up1() {
    // Limpiar la posición actual del sprite antes de moverlo
    clear_previous_sprite(sprite_x1, sprite_y1, sprite_width1, sprite_height1);

    // Verificar si no se sale del límite superior antes de mover
    if (sprite_y1 - move_step1 >= y1_limit_min) {
        sprite_y1 -= move_step1;  // Mover hacia arriba
    }

    // Redibujar el sprite en la nueva posición
    LCD_Sprite(sprite_x1, sprite_y1, sprite_width1, sprite_height1, seleccionJug2, 2, anima2, 0, 1);  // Dibuja el sprite en su nueva posición
    //LCD_Bitmap(sprite_x, sprite_y, sprite_width, sprite_height, Jug1);
    if (check_collision(sprite_x1, sprite_y1, z, 50)) { //check_collision(sprite_x, sprite_y, green_rect_x, green_rect_y
        	colision2++;
    }

    // Mostrar el contador de colisiones en pantalla
    /*sprintf(Pant, "Colisiones: %d", colision);
    LCD_Print(Pant, 20, 100, 1, 0x001F, 0xEDCC);*/  // Imprimir colisiones en la pantalla

}

void move_sprite_down1() {
    // Limpiar la posición actual del sprite antes de moverlo
    clear_previous_sprite(sprite_x1, sprite_y1, sprite_width1, sprite_height1);

    // Verificar si no se sale del límite inferior antes de mover
    if (sprite_y1 + move_step1 + sprite_height1 <= y1_limit_max) {
        sprite_y1 += move_step1;  // Mover hacia abajo
    }

    // Redibujar el sprite en la nueva posición
    LCD_Sprite(sprite_x1, sprite_y1, sprite_width1, sprite_height1, seleccionJug2, 2, anima2, 0, 1);  // Dibuja el sprite en su nueva posición

    //LCD_Bitmap(sprite_x, sprite_y, sprite_width, sprite_height, Jug1);
    if (check_collision(sprite_x1, sprite_y1, z, 50)) {
    	colision2++;
    }

    // Mostrar el contador de colisiones en pantalla
    /*sprintf(Pant, "Colisiones: %d", colision);
    LCD_Print(Pant, 20, 100, 1, 0x001F, 0xEDCC);  // Imprimir colisiones en la pantalla*/
}




//Función para leer de la SD





/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	//srand(time(NULL));
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
  MX_UART5_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  	//HAL_UART_Transmit_DMA(&huart3, musica, sizeof(musica)); //Envia los datos del buffer musica via RX en DMA
    HAL_UART_Receive_DMA(&huart3, data, 1); //Recibir comandos via uart en DMA
    HAL_UART_Receive_DMA(&huart5, data2, 1);
    HAL_UART_Receive_DMA(&huart2, data, 1); //Comentar luego de pruebas
  	//HAL_UART_Receive_IT(&huart3, data, 1);
	LCD_Init();

	void transmit_uart(char *string){
		  uint8_t len = strlen(string);
		  HAL_UART_Transmit(&huart5, (uint8_t*) string, len, 200); //200
	}

	void transmit_uart2(char *string){
			  uint8_t len = strlen(string);
			  HAL_UART_Transmit(&huart3, (uint8_t*) string, len, 200); //200
	}

	//leer_archivo();


	/*fres = f_mount(&fs, "/", 0);
	if(fres == FR_OK){
		//LCD_Print("Micro SD card is mounted succesfully!", 20, 50, 1, 0x001F, 0xCAB9);
	}else if(fres != FR_OK){
		//LCD_Print("Micro SD card's mount error!", 20, 100, 1, 0x001F, 0xCAB9);
	}
	fres = f_open(&fil, "change1.txt", FA_READ);
	if(fres == FR_OK){
		//LCD_Print("File opened for reading.", 20, 100, 1, 0x001F, 0xCAB9);
		leer = 1;
	}else if(fres != FR_OK){
		//LCD_Print("File was not opened for reading!", 30, 100, 1, 0x001F, 0xCAB9);
		leer = 0;
	}
	if (leer == 1){
		while(f_gets(buffer, sizeof(buffer), &fil)){
				//Leer valores iniciando en 0 hasta llegar a , o espacio y luego concatenar todos los datos en el buffer
		}
		leer = 0;
	}
	fres = f_close(&fil);
	if(fres == FR_OK){
		//LCD_Print("The file is closed", 30, 150, 1, 0x001F, 0xCAB9);
	}else if(fres != FR_OK){
		//LCD_Print("The file was not closed", 30, 150, 1, 0x001F, 0xCAB9);
	}
	f_mount(NULL,"",1);
	LCD_Bitmap(10, 60, 35, 38, buffer);
	memset(buffer, '\0', 16000);*/

	uint8_t imageBuffer[8000];


	/*fres = f_mount(&fs, "/", 0);
	if (fres == FR_OK) {
	    // Montaje correcto de la tarjeta SD
	} else {
	    // Error al montar la tarjeta SD
	}

	fres = f_open(&fil, "change1.txt", FA_READ);
	if (fres == FR_OK) {
	    // Archivo abierto correctamente para lectura
	    leer = 1;
	} else {
	    // Error al abrir el archivo
	    leer = 0;
	}

	if (leer == 1) {
	    char buffer[16000];  // Buffer para leer datos del archivo
	    char temp[5];        // Para almacenar temporalmente cada "0xXX"
	    int index = 0;       // �?ndice para guardar el valor en el buffer binario
	    uint8_t imageBuffer[16000]; // Buffer de imagen en formato binario
	    UINT bytesLeidos;

	    while (f_gets(buffer, sizeof(buffer), &fil)) {
	        char *ptr = buffer;
	        while (*ptr != '\0') {
	            if (*ptr == '0' && *(ptr + 1) == 'x') {
	                // Extrae los dos caracteres después de "0x"
	                temp[0] = *(ptr + 2);
	                temp[1] = *(ptr + 3);
	                temp[2] = '\0';

	                // Convierte la cadena "XX" en un valor hexadecimal
	                uint8_t value = (uint8_t)strtol(temp, NULL, 16);
	                imageBuffer[index++] = value;

	                // Aumenta el puntero para saltar los "0xXX"
	                ptr += 4;
	            } else {
	                // Avanza el puntero para ignorar comas, espacios u otros caracteres
	                ptr++;
	            }
	        }
	    }
	    leer = 0;
	}

	fres = f_close(&fil);
	if (fres == FR_OK) {
	    // Archivo cerrado correctamente
	} else {
	    // Error al cerrar el archivo
	}

	f_mount(NULL, "", 1);*/


	/*fres = f_mount(&fs, "/", 0);
	if (fres == FR_OK) {
	    // Tarjeta SD montada correctamente
	} else {
	    // Error al montar la tarjeta SD
	}

	fres = f_open(&fil, "change1.txt", FA_READ);
	if (fres == FR_OK) {
	    // Archivo abierto correctamente
	    leer = 1;
	} else {
	    // Error al abrir el archivo
	    leer = 0;
	}

	if (leer == 1) {
	    char buffer[16000];  // Buffer para leer datos del archivo
	    char temp[5];        // Buffer temporal para almacenar cada "0xXXXX"
	    int index = 0;       // �?ndice para guardar el valor en el buffer binario
	    uint16_t imageBuffer[8000]; // Buffer de imagen en formato binario RGB565 (16 bits por píxel)
	    UINT bytesLeidos;
	    int dataCount = 0;   // Contador de datos procesados

	    // Limpiar buffers antes de usar
	    memset(buffer, 0, sizeof(buffer));
	    memset(imageBuffer, 0, sizeof(imageBuffer));

	    while (f_gets(buffer, sizeof(buffer), &fil)) {
	        char *ptr = buffer;
	        while (*ptr != '\0') {
	            if (*ptr == '0' && *(ptr + 1) == 'x') {
	                // Extrae los 4 caracteres después de "0x"
	                temp[0] = *(ptr + 2);
	                temp[1] = *(ptr + 3);
	                temp[2] = *(ptr + 4);
	                temp[3] = *(ptr + 5);
	                temp[4] = '\0';

	                // Convierte la cadena "XXXX" en un valor hexadecimal de 16 bits
	                uint16_t colorValue = (uint16_t)strtol(temp, NULL, 16);

	                // Guardar el valor en el buffer de la imagen
	                imageBuffer[index++] = colorValue;
	                dataCount++;  // Contar el número de valores leídos

	                // Avanza el puntero para saltar los "0xXXXX"
	                ptr += 6;
	            } else {
	                // Ignorar otros caracteres (coma, espacios, etc.)
	                ptr++;
	            }

	            // Evitar sobrepasar el tamaño del buffer
	            if (index >= sizeof(imageBuffer) / sizeof(imageBuffer[0])) {
	                break;
	            }
	        }

	        // Verificar si alcanzamos el tamaño esperado de la imagen
	        if (index >= sizeof(imageBuffer) / sizeof(imageBuffer[0])) {
	            break;
	        }
	    }

	    // Verificar que se leyeron suficientes datos
	    if (dataCount < 8000) {
	        // Error: no se leyeron suficientes datos
	        // Aquí puedes imprimir un mensaje de error en tu LCD o serie
	        // LCD_Print("Error: No se leyeron suficientes datos.", 10, 150, 1, 0xF800, 0xFFFF);
	    } else {
	        // Datos suficientes leídos
	    }

	    leer = 0;
	}

	fres = f_close(&fil);
	if (fres == FR_OK) {
	    // Archivo cerrado correctamente
	} else {
	    // Error al cerrar el archivo
	}

	f_mount(NULL, "", 1);*/

	// Muestra la imagen desde el buffer de imagen binario
	// Ajusta las dimensiones según el tamaño de tu imagen (ejemplo 160x100)



	LCD_Clear(0xEE93);

	LCD_Bitmap(70, 35, 180, 68, exciteinicio);
	LCD_Bitmap(45, 105, 234, 45, DESERT);
	LCD_Bitmap(15, 200, 201, 22, LOGONINTENDO);

	//Funciona activar musica
	//transmit_uart('1');





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {

		if(received_char == 'a' && unico == 1){
			start = 1;
			unico = 0;
			received_char = 'o';
		}
		if(start == 1){
			LCD_Clear(0x2817);
			LCD_Bitmap(53, 25, 214, 85, LOGOEX);
			LCD_Bitmap(110, 130, 98, 12, OPCION1);
			LCD_Bitmap(110, 162, 142, 13, OPCION2);
			//LCD_Bitmap(110, 195, 84, 11, OPCION3);
			LCD_Bitmap(80, 130, 14, 15, PUNTERO);
			start = 0;
			inicio = 3;
			seleccion_actual = 1;
		}
		if(inicio == 3 && received_char != new_command){ //
			//HAL_Delay(200);
			if(received_char == 'd'){
				mover_puntero_arriba();
				received_char = 'o';
			}
			else if(received_char == 'c'){
				mover_puntero_abajo();
				received_char = 'o';
			}

			new_command == received_char;
		}

		if(received_char == 'a' && seleccion_actual == 1){
			inicio = 0;
			seleccion_actual = 0;
			pintjueg = 1;
			received_char = 'o';
		}
		if(received_char == 'a' && seleccion_actual == 2){
			inicio = 1;
			seleccion_actual = 0;
			pintjueg = 1;
			received_char = 'o';
		}
		if(received_char == 'a' && seleccion_actual == 3){
			inicio = 2;
			seleccion_actual = 0;
			pintjueg = 1;
			received_char = 'o';
		}

		switch (inicio){
			case 0:
				transmit_uart('1');
				if(pintjueg == 1){
					LCD_Clear(0xEDCC);


					LCD_Bitmap(0, 110, 42,15, MURO);
					LCD_Bitmap(40, 110, 42,15, MURO);
					LCD_Bitmap(80, 110, 42,15, MURO);
					LCD_Bitmap(120, 110, 42,15, MURO);
					LCD_Bitmap(160, 110, 42,15, MURO);
					LCD_Bitmap(200, 110, 42,15, MURO);
					LCD_Bitmap(240, 110, 42,15, MURO);
					LCD_Bitmap(280, 110, 42,15, MURO);


					/*LCD_Bitmap(5, 5, 22,19, ICONJ1);
					LCD_Bitmap(9, 8, 12,12, LETRAJ1);

					LCD_Bitmap(5, 215, 22,19, ICONJ2);
					LCD_Bitmap(7, 218, 16,12, LETRAJ2);*/

					LCD_Bitmap(5, 215, 22,19, ICONJ1);
					LCD_Bitmap(9, 218, 12,12, LETRAJ1);

					LCD_Bitmap(5, 5, 22,19, ICONJ2);
					LCD_Bitmap(7, 8, 16,12, LETRAJ2);




					//HEALTH BAR J2
					FillRect(26, 9, 35, 10, 0xBBE0);
					FillRect(61, 9, 35, 10, 0xBBE0);
					FillRect(96, 9, 35, 10, 0xBBE0);
					FillRect(131, 9, 35, 10, 0xBBE0);

					//HEALTH BAR J2
					FillRect(26, 219, 35, 10, 0x321B);
					FillRect(61, 219, 35, 10, 0x321B);
					FillRect(96, 219, 35, 10, 0x321B);
					FillRect(131, 219, 35, 10, 0x321B);
					pintjueg = 0;

				}

				//musica[0] = 'b';


				tiempo1++;
				tiempo2++;

				if(selector1 == 0 && tiempo1 >= last_time_i){
					clear_previous_sprite(z, 130, 22, 25);
					int anima1 = (z / 4) % 4;  // Animación para el primer monstruo
										// Dibujar los monstruos en la pantalla
					LCD_Sprite(z, 130, 21, 24, giroPOKEY, 4, anima1, 0, 1); // Primer monstruo
										// Incrementar las coordenadas para que los monstruos se muevan
					z--; // Incremento en X para el primer monstruo
					if (check_collision(sprite_x, sprite_y, z, 150)) {
						colision2++;
						boom2 = 1;
						// Borrar el cactus cuando hay colisión
						FillRect(z, 130, 22, 25, 0xEDCC);
						z = 300;
						FillRect(z, 130, 22, 25, 0xEDCC); // Rellenar con color de fondo
						selector1 = 1;
						//musica[0] = 'c';
						transmit_uart('3');
						transmit_uart2("1");
					}
					if (z == 0){
						FillRect(z, 130, 22, 25, 0xEDCC); // Reiniciar la posición del enemigo
						z = 300;
						selector1 = 1;
						//tiempo = 0;
						tiempo1 = 0;
					}
				}

				if(selector2 == 0 && tiempo2 >= last_time_i){
					clear_previous_sprite(u, 30, 22, 25);
					int anima3 = (u / 4) % 4;  // Animación para el segundo monstruo
					LCD_Sprite(u, 30, 21, 24, giroPOKEY, 4, anima3, 0, 1);      // Segundo monstruo
					u--;
					if(check_collision(sprite_x1, sprite_y1, u, 30)){
						colision++;
						boom1 = 1;
						FillRect(u, 30, 22, 25, 0xEDCC);
						u = 300;
						FillRect(u, 30, 22, 25, 0xEDCC);
						selector2 = 1;
						//musica[0] = 'c';
						transmit_uart('3');
						transmit_uart2("1");
					}
					if(u == 0){
						FillRect(u, 30, 22, 25, 0xEDCC);
						u = 300;
						selector2 = 1;
						tiempo2 = 0;
					}
				}


				if(selector1 == 1 && tiempo1 >= last_time_i){
					clear_previous_sprite(z, 180, 35, 25);
					int anima1 = (z / 3) % 3;  // Animación para el primer monstruo
					// Dibujar los monstruos en la pantalla
					LCD_Sprite(z, 180, 34, 25, CAMEL, 3, anima1, 0, 1); // Primer monstruo
					// Incrementar las coordenadas para que los monstruos se muevan
					z--; // Incremento en X para el primer monstruo
					if (check_collision(sprite_x, sprite_y, z, 180)) {
						colision2++;
						boom2 = 1;
						// Borrar el cactus cuando hay colisión
						FillRect(z, 180, 34, 25, 0xEDCC);
						z = 300;
						FillRect(z, 180, 34, 25, 0xEDCC); // Rellenar con color de fondo
						selector1 = 0;
						//musica[0] = 'c';
						transmit_uart('3');
						transmit_uart2("1");
					}
					if (z == 0){
						FillRect(z, 180, 34, 25, 0xEDCC); // Reiniciar la posición del enemigo
						z = 300;
						selector1 = 0;
						tiempo1 = 0;
					}
				}

				if(selector2 == 1 && tiempo2 >= last_time_i){
					clear_previous_sprite(u, 80, 35, 25);
					int anima3 = (u / 3) % 3;  // Animación para el segundo monstruo
					LCD_Sprite(u, 80, 34, 25, CAMEL, 3, anima3, 0, 1);      // Segundo monstruo
					u--;
					if(check_collision(sprite_x1, sprite_y1, u, 70)){
						colision++;
						boom1 = 1;
						FillRect(u, 80, 34, 25, 0xEDCC);
						u = 300;
						FillRect(u, 80, 34, 25, 0xEDCC);
						selector2 = 0;
						//musica[0] = 'c';
						transmit_uart('3');
						transmit_uart2("1");
					}
					if(u == 0){
						FillRect(u, 80, 34, 25, 0xEDCC);
						u = 300;
						selector2 = 0;
						tiempo2 = 0;
					}
				}


				if(colision == 1){
					if(boom1 == 1){
						for(int x = 0; x < 80; x++){
							HAL_Delay(15);
							int anima = (x/35)%3;
							LCD_Sprite(sprite_x1, sprite_y1, 37, 37, caida2, 3, anima, 0, 1);
						}
						boom1 = 0;
					}
					LCD_Sprite(sprite_x1, sprite_y1, 34, 37, seleccionJug2, 2, anima2, 0, 1);
					FillRect(131, 9, 35, 10, 0xEDCC);
				}else if(colision == 2){
					if(boom1 == 1){
						for(int x = 0; x < 80; x++){
							HAL_Delay(15);
							int anima = (x/35)%3;
							LCD_Sprite(sprite_x1, sprite_y1, 37, 37, caida2, 3, anima, 0, 1);
						}
						boom1 = 0;
					}
					LCD_Sprite(sprite_x1, sprite_y1, 34, 37, seleccionJug2, 2, anima2, 0, 1);
					FillRect(96, 9, 35, 10, 0xEDCC);
				}else if(colision == 3){
					if(boom1 == 1){
						for(int x = 0; x < 80; x++){
							HAL_Delay(15);
							int anima = (x/35)%3;
							LCD_Sprite(sprite_x1, sprite_y1, 37, 37, caida2, 3, anima, 0, 1);
						}
						boom1 = 0;
					}
					LCD_Sprite(sprite_x1, sprite_y1, 34, 37, seleccionJug2, 2, anima2, 0, 1);
					FillRect(61, 9, 35, 10, 0xEDCC);
				}else if(colision == 4){
					if(boom1 == 1){
						for(int x = 0; x < 80; x++){
							HAL_Delay(15);
							int anima = (x/35)%3;
							LCD_Sprite(sprite_x1, sprite_y1, 37, 37, caida2, 3, anima, 0, 1);
						}
						boom1 = 0;
					}
					LCD_Sprite(sprite_x1, sprite_y1, 34, 37, seleccionJug2, 2, anima2, 0, 1);
					FillRect(26, 9, 35, 10, 0xEDCC);
					loser2 = 1;
				}


				if(colision2 == 1){

					if(boom2 == 1){
						/*int x = 0;
						x++;
						int anima = (x/50)%3;
						LCD_Sprite(sprite_x, sprite_y, 31, 32, colijug1, 3, anima, 0, 1);
						if (x >= 320 - 32) {
							x = 0; // Restablece x para futuras ejecuciones
						}*/
						for(int x = 0; x < 80; x++){
							HAL_Delay(15);
							int anima = (x/35)%3;
							//LCD_Sprite(sprite_x, sprite_y, 31, 32, colisionjug1, 3, anima, 0, 1);
							LCD_Sprite(sprite_x, sprite_y, 37, 37, caida1, 3, anima, 0, 1);
						}
						boom2 = 0;
					}
					//LCD_Bitmap(sprite_x, sprite_y, sprite_width, sprite_height, Jug1);
					LCD_Sprite(sprite_x, sprite_y, 34, 37, seleccionJug1, 2, anima1, 0, 1);
					FillRect(131, 219, 35, 10, 0xEDCC);

				}else if(colision2 == 2){
					if(boom2 == 1){
						/*int x = 0;
						x++;
						int anima = (x/50)%3;
						LCD_Sprite(sprite_x, sprite_y, 31, 32, colijug1, 3, anima, 0, 1);
						if (x >= 320 - 32) {
							x = 0; // Restablece x para futuras ejecuciones
						}*/
						for(int x = 0; x < 80; x++){
							HAL_Delay(15);
							int anima = (x/35)%3;
							LCD_Sprite(sprite_x, sprite_y, 37, 37, caida1, 3, anima, 0, 1);
						}
						boom2 = 0;
					}
					//LCD_Bitmap(sprite_x, sprite_y, sprite_width, sprite_height, Jug1);
					LCD_Sprite(sprite_x, sprite_y, 34, 37, seleccionJug1, 2, anima1, 0, 1);
					FillRect(96, 219, 35, 10, 0xEDCC);
				}else if(colision2 == 3){
					if(boom2 == 1){
						for(int x = 0; x < 80; x++){
							HAL_Delay(15);
							int anima = (x/35)%3;
							LCD_Sprite(sprite_x, sprite_y, 37, 37, caida1, 3, anima, 0, 1);
						}
						boom2 = 0;
					}
					LCD_Sprite(sprite_x, sprite_y, 34, 37, seleccionJug1, 2, anima1, 0, 1);
					FillRect(61, 219, 35, 10, 0xEDCC);
				}else if(colision2 == 4){
					if(boom2 == 1){
						for(int x = 0; x < 80; x++){
							HAL_Delay(15);
							int anima = (x/35)%3;
							LCD_Sprite(sprite_x, sprite_y, 37, 37, caida1, 3, anima, 0, 1);
						}
						boom2 = 0;
					}
					LCD_Sprite(sprite_x, sprite_y, 34, 37, seleccionJug1, 2, anima1, 0, 1);
					FillRect(26, 219, 35, 10, 0xEDCC);
					loser1 = 1;
				}



				//Banderas de perdedores para cambio a podio
				if(loser1 == 1){
					//LCD_Print("Perdiste! JUG1 :(", 20, 100, 1, 0x001F, 0xCAB9);
					inicio = 4;
				}
				if(loser2 == 1){
					//LCD_Print("Perdiste! JUG2 :(", 20, 100, 1, 0x001F, 0xCAB9);
					inicio = 4;
				}

				//Movimiento Jug2
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

				//Movimiento Jug2
				if (received_char2 == 'i') {
					//move_green_cube_left();  // Mover cubo verde hacia la izquierda
					move_sprite_up1();

					received_char2 = 0;  // Resetear el comando
					move2 = 0;
				}
				if(received_char2 == 'j'){
					//move_green_cube_right();
					move_sprite_down1();

					received_char2 = 0;
					move2 = 0;
				}

				/*c++;
				int anima4 = (c / 2) % 2;
				LCD_Sprite(sprite_x1, sprite_y1, 34, 37, seleccionJug2, 2, anima4, 0, 1);

				m++;
				int anima2 = (m / 2) % 2;
				LCD_Sprite(sprite_x, sprite_y, 34, 37, seleccionJug1, 2, anima2, 0, 1);*/

				//Movimiento con comandos
				if(move2 != 1){
					c++;
					int anima4 = (c / 2) % 2;
					LCD_Sprite(sprite_x1, sprite_y1, 34, 37, seleccionJug2, 2, anima4, 0, 1);
				}
				if(move1 != 1){
					m++;
					int anima2 = (m / 2) % 2;
					LCD_Sprite(sprite_x, sprite_y, 34, 37, seleccionJug1, 2, anima2, 0, 1);
				}
				if(c == 20){
					c = 0;
				}else if(m == 20){
					m = 0;
				}
				break;
			case 1:
				if(pintjueg == 1){
					LCD_Clear(0x00A7);


					//             *       *

					LCD_Bitmap(25, 80, 63, 69, SELECTOROFF);

					//LCD_Bitmap(40, 90, 35, 38, buffer);
					LCD_Bitmap(40, 90, 35, 38, personaje1);

					//memset(buffer, '\0', 100); //Limpia el buffer

					LCD_Bitmap(95, 80, 63, 69, SELECTOROFF);
					LCD_Bitmap(110, 90, 35, 38, personaje2);

					LCD_Bitmap(165, 80, 63, 69, SELECTOROFF);
					LCD_Bitmap(180, 90, 35, 38, personaje3);

					LCD_Bitmap(235, 80, 63, 69, SELECTOROFF);
					LCD_Bitmap(250, 90, 35, 38, personaje4);


					//Añadiendo barra gris del selector del jugador
					LCD_Bitmap(25, 70, 63, 17, SELECTOROFFJ1);
					LCD_Bitmap(95, 70, 63, 17, SELECTOROFFJ1);
					LCD_Bitmap(165, 70, 63, 17, SELECTOROFFJ1);
					LCD_Bitmap(235, 70, 63, 17, SELECTOROFFJ1);

					//80+69-18 REFERENCIA PERSONAJE 1 (Poner la misma coordenada en x que SELECTOROFF del personaje)

					LCD_Bitmap(25, 70, 63, 17, SELECTORONJ1);

					//80+69-18 REFERENCIA PERSONAJE 4
					LCD_Bitmap(235, 131, 63, 17, SELECTORONJ2);

					LCD_Bitmap(60, 175, 200, 12, AVISOJUGADOR);



					pintjueg = 0;
				}
				if(received_char != new_command){
					mover_colores(received_char);
					new_command = received_char;
				}
				// Dependiendo de la posición del selector, se asigna la animación correspondiente
				if(received_char == 'a'){
					if (selector1_pos == 1) {
						seleccionJug1 = movjug1;
						caida1 = colisionjug1;
					} else if (selector1_pos == 2) {
						seleccionJug1 = movjug2;
						caida1 = colisionjug2;
					} else if (selector1_pos == 3) {
						seleccionJug1 = movjug3;
						caida1 = colisionjug3;
					} else if (selector1_pos == 4) {
						seleccionJug1 = movjug4;
						caida1 = colisionjug4;
					}

					if (selector2_pos == 1) {
						seleccionJug2 = movjug1;
						caida2 = colisionjug1;
					} else if (selector2_pos == 2) {
						seleccionJug2 = movjug2;
						caida2 = colisionjug2;
					} else if (selector2_pos == 3) {
						seleccionJug2 = movjug3;
						caida2 = colisionjug3;
					} else if (selector2_pos == 4) {
						seleccionJug2 = movjug4;
						caida2 = colisionjug4;
					}
					start = 1;
					received_char = 'o';
				}
				if(received_char == 'c'){
					start = 1;
					inicio = 5;
					received_char = 'o';
				}

				//Implementar lectura boton 'b' regreso a menu , start = 1, logica seria mover selectores JUG1 y JUG2 guardar selecciones para imprimir en juego
				break;
			case 2:
				if(pintjueg == 1){
					LCD_Clear(0xFFFF);
					sprintf(Pant, "Colisiones: %d", colision);
					LCD_Print(Pant, 20, 100, 1, 0x001F, 0xEDCC);
					pintjueg = 0;
				}

				break;
			case 4:
				if(loser2 == 1){
					//Podio ganador1
					LCD_Clear(0x0015);
					LCD_Bitmap(75, 85, 173, 119, podio);
					LCD_Bitmap(115, 10, 96, 84, trofeo);
					LCD_Bitmap(25, 205, 274, 25, aviso1);
					loser2 = 0;
					//inicio = 3;
				}
				if(loser1 == 1){
					//Podio ganador
					LCD_Clear(0x0015);
					LCD_Bitmap(75, 85, 173, 119, podio);
					LCD_Bitmap(115, 10, 96, 84, trofeo);
					LCD_Bitmap(25, 205, 274, 25, aviso2);
					loser1 = 0;
					//inicio = 3;
				}
				if(received_char == 'c'){
					start = 1;
					inicio = 5;
					received_char = 'o';
				}

				break;
			default:
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
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 9600;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  huart3.Init.Mode = UART_MODE_TX_RX;
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
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream1_IRQn);
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

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
  __HAL_RCC_GPIOD_CLK_ENABLE();

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

	//HAL_UART_Receive_IT(&huart3, data, 1);
	if(huart->Instance == USART3){

		HAL_UART_Receive_DMA(&huart3, data, 1);
		received_char = data[0];
		move1 = 1;
	}

	if(huart->Instance == UART5){
		HAL_UART_Receive_DMA(&huart5, data2, 1);
		received_char2 = data2[0];
		move2 = 1;
	}

	if(huart->Instance == USART2){
		HAL_UART_Receive_DMA(&huart2, data, 1);
		//received_char2 = data2[0];
		received_char = data[0];
	}




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
