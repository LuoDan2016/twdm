#ifndef _BOARD_H_
#define _BOARD_H_

//Driver includes
#include "driver/spi_master.h"          //Include for SPI functionality
#include "lcd.h"                        //Inclde for LCD functionality

//SPI config
#define BOARD_INT_SPI_HOST          HSPI_HOST
#define BOARD_INT_SPI_MISO          12
#define BOARD_INT_SPI_MOSI          13
#define BOARD_INT_SPI_CLK           14

//Globals

//Function definitions
void board_init();

//Struct definitions

//Constants

#endif
