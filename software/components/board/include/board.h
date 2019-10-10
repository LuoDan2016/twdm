#ifndef _BOARD_H_
#define _BOARD_H_

//Driver includes
#include "driver/spi_master.h"          //Include for SPI functionality
#include "lcd.h"                        //Inclde for LCD functionality

//SPI config
#define BOARD_SPI1_HOST         HSPI_HOST
#define BOARD_SPI1_PIN_MISO     12
#define BOARD_SPI1_PIN_MOSI     13
#define BOARD_SPI1_PIN_CLK      14

//Globals

//Function definitions
void board_init();

//Struct definitions

//Constants

#endif
