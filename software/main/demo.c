/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"

#include "esp_vfs_fat.h"
#include "driver/sdmmc_host.h"
#include "driver/sdspi_host.h"
#include "sdmmc_cmd.h"

#include "board.h"
#include "lcd.h"
#include "font.h"
//#include "touch.h"

//IMPORTANT: see edit in driver/sdspi_host.c
//#include "sdcard.h"

// static void IRAM_ATTR touch_isr_handler(void* arg){
//     ESP_EARLY_LOGI("TIR", "Touch triggered!");
// }

void app_main(){
    printf("Hello world!\n");




    //board_init();
    //lcd_init();
    //Simple clear routine
    //uint16_t bl = lcd_get_color(0, 0, 0);
    // for(uint16_t x=0; x < 320L; x++){
    //     for(uint16_t y=0; y < 16L; y++){
    //         lcd_draw_pixel(x, y, bl);
    //     }
    // }
    // lcd_draw_str(0, 0, "SCREENTEST OK");

    //Do other stuff
    //vTaskDelay(5000 / portTICK_PERIOD_MS);
    //bool res = sdcard_init();


    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot           = BOARD_INT_SPI_HOST;
    host.max_freq_khz   = 10000;

    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = BOARD_INT_SPI_MISO;
    slot_config.gpio_mosi = BOARD_INT_SPI_MOSI;
    slot_config.gpio_sck  = BOARD_INT_SPI_CLK;
    slot_config.gpio_cs   = 25;
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
       .format_if_mount_failed = false,
       .max_files = 5,
       .allocation_unit_size = 16 * 1024
    };
    sdmmc_card_t* card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    sdmmc_card_print_info(stdout, card);

    if (ret != ESP_OK) {
        if (ret == ESP_FAIL) {
            ESP_LOGE("SDCARD", "Failed to mount filesystem. "
                "If you want the card to be formatted, set format_if_mount_failed = true.");
        } else {
            ESP_LOGE("SDCARD", "Failed to initialize the card (%s). "
                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
        }
    }
    else{
        //mount /sdcard is now useable
        FILE* f = fopen("/sdcard/myfile.txt", "r");
        if (f == NULL) {
            //lcd_draw_str(0, 1, "COULD NOT READ");
            ESP_LOGE("SDCARD", "COULD NOT READ");
        }
        char line[64];
        fgets(line, sizeof(line), f);
        fclose(f);
        // strip newline
        char* pos = strchr(line, '\n');
        if (pos) {
            *pos = '\0';
        }
        //lcd_draw_str(0, 1, line);
        printf("read: %s\n\n", line);

        //write
        f = fopen("/sdcard/hello.txt", "w");
        if (f == NULL) {
            //lcd_draw_str(0, 1, "COULD NOT WRITE");
            ESP_LOGE("SDCARD", "COULD NOT WRITE");
            return;
        }
        fprintf(f, "ASDEF ASDEF test\nNewline\nanoteh");
        fclose(f);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS);



    //touch_init();
    //touch_irq_init(touch_isr_handler);

    //
    // uint16_t r = lcd_get_color(255, 0, 0);
    //

    // vTaskDelay(2000 / portTICK_PERIOD_MS);



    // vTaskDelay(2000 / portTICK_PERIOD_MS);
    //
    // touch_point_t p;
    // memset(&p, 0, sizeof(p));
    // while(1){
    //     vTaskDelay(20 / portTICK_PERIOD_MS);
    //     touch_get_point(&p);
    //     printf("x: %i, y: %i\n", p.x, p.y);
    //     if(p.x > 0 && p.y > 0) lcd_draw_pixel(p.x, p.y, r);
    // }
    //vTaskDelay(2000 / portTICK_PERIOD_MS);
    //lcd_draw_str(0, 0, "POWER DOWN           ");
    vTaskDelay(20000 / portTICK_PERIOD_MS);
    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
