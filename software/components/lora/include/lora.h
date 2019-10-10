#ifndef _LORA_H_
#define _LORA_H_

//Driver includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "board.h"                      //Include for defines in board.h

//SPI config
#define LORA_MHZ                4
#define LORA_PIN_CS             4 //TODO: change

//Globals
spi_device_handle_t LORA_SPI_HANDLE;

//Struct definitions

//Functions definitions
//Init
void lora_init();

#endif
