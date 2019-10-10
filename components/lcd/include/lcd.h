#ifndef _LCD_H_
#define _LCD_H_

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
#define LCD_MHZ                 40
#define LCD_PIN_CS              15
#define LCD_PIN_DC              27

//Globals
spi_device_handle_t LCD_SPI_HANDLE;

//Functions definitions
//Internally used
void lcd_pre_transfer_callback(spi_transaction_t *t);
//Init
void lcd_init();
//Utility
void lcd_data(const uint8_t *data, int len);
void lcd_cmd(const uint8_t cmd);
uint16_t lcd_get_color(uint8_t r, uint8_t g, uint8_t b);
//Memory management
void lcd_setwin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
//Drawing functions
void lcd_draw_pixel(uint16_t x0, uint16_t y0, uint16_t color);

//Struct definitions
typedef struct {
    uint8_t cmd;        //Command byte
    uint8_t databytes;  //Num data bytes, MSB sets delay, 0xFF is end
    uint8_t data[16];   //Data to send
} lcd_cmd_list_t;

//Constants
DRAM_ATTR static const lcd_cmd_list_t lcd_init_cmds[] = {
    {0x01,  0x80,   {0}},                               //Software reset
    {0xCB,  5,      {0x39, 0x2c, 0x00, 0x34, 0x02}},
    {0xCF,  3,      {0x00, 0xc1, 0x30}},
    {0xEF,  3,      {0x03, 0x80, 0x02}},
    {0xE8,  3,      {0x85, 0x00, 0x78}},
    {0xEA,  2,      {0x00, 0x00}},
    {0xED,  4,      {0x64, 0x03, 0x12, 0x81}},
    {0xF7,  1,      {0x20}},
    {0xC0,  1,      {0x23}},                            // Power control VRH[5:0]
    {0xC1,  1,      {0x10}},                            // Power control SAP[2:0];BT[3:0]
    {0xC5,  2,      {0x3e, 0x28}},                      // VCM control
    {0xC6,  1,      {0x86}},                            // VCM control2
    {0x36,  1,      {0x20}},                            // Memory Access Control (orientation)
    {0x3A,  1,      {0x55}},                            // Pixel format 16 bit
    {0x20,  0,      {0}},
    {0xB1,  2,      {0x00, 0x13}},                      // 0x18 79Hz, 0x1B default 70Hz, 0x13 100Hz
    {0xB6,  4,      {0x08, 0x82, 0x27, 0x00}},          // Display Function Control
    {0x30,  4,      {0x00, 0x00, 0x01, 0x3F}},
    {0xF2,  1,      {0x00}},                            // 3Gamma Function: Disable (0x02), Enable (0x03)
    {0x26,  1,      {0x01}},                            // Gamma curve selected (0x01, 0x02, 0x04, 0x08)
    {0xE0,  0x8F,   {0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E,0xF1,  0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00}},
    {0xE1,  0x8F,   {0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31,0xC1,  0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F}},
    {0x11,  0,      {0}},                               // Sleep out
    {0x29,  0,      {0}},                               // Display on
    {0x00,  0xFF,   {0}}                                // End of sequence.
};

#endif
