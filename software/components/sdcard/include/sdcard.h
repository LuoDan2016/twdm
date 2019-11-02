#ifndef _SDCARD_H_
#define _SDCARD_H_

//Driver includes
#include <stdio.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "esp_err.h"
#include "esp_log.h"
#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
//Important: use our own sdspi driver that allows spi master sharing
#include "driver/sdspi_host.h"

#include "sdmmc_cmd.h"
#include "board.h"                      //Include for defines in board.h

//SPI config
//#define SDCARD_MHZ               2
#define SDCARD_PIN_CS           25

//Globals
spi_device_handle_t SDCARD_SPI_HANDLE;

bool sdcard_init();

#endif
