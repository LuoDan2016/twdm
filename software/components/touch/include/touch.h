#ifndef _TOUCH_H_
#define _TOUCH_H_

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
#define TOUCH_MHZ               2
#define TOUCH_PIN_CS            0       //TODO: Change, also do in callbacks maybe?
#define TOUCH_PIN_IRQ           36      //TODO: Change

//Globals
spi_device_handle_t TOUCH_SPI_HANDLE;

//Struct definitions
typedef struct {
    uint16_t x;
    uint16_t y;
} touch_point_t;

//Functions definitions
//Init
void touch_init();

uint8_t touch_trans_cmd8(uint8_t cmd);
uint16_t touch_trans_cmd16(uint16_t cmd);

void touch_get_point(touch_point_t *p);

#endif
