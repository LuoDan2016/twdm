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

#include "board.h"
#include "lcd.h"
#include "font.h"
#include "touch.h"

void app_main(){
    printf("Hello world!\n");
    vTaskDelay(5000 / portTICK_PERIOD_MS);

    board_init();
    lcd_init();
    touch_init();

    uint16_t bl = lcd_get_color(0, 0, 0);
    uint16_t r = lcd_get_color(255, 0, 0);
    // uint16_t g = lcd_get_color(0, 255, 0);
    // uint16_t b = lcd_get_color(0, 0, 255);
    //
    for(uint16_t x=0; x < 320L; x++){
        for(uint16_t y=0; y < 240L; y++){
            lcd_draw_pixel(x, y, bl);
        }
    }
    // printf("Clear done\n");
    // vTaskDelay(1000 / portTICK_PERIOD_MS);
    // lcd_draw_pixel(10, 20, r);
    // lcd_draw_pixel(10, 35, g);
    // lcd_draw_pixel(10, 50, b);
    // printf("Pixeldraw done\n");
    //
    // vTaskDelay(2000 / portTICK_PERIOD_MS);
    //uint16_t r = lcd_get_color(255, 0, 0);
    //lcd_draw_str(0, 0, "Did you ever hear the tragedy of Darth Plagueis the Wise? I thought not. It's not a story the Jedi would tell you. It's a Sith legend. Darth Plagueis was a Dark Lord of the Sith, so powerful and so wise he could use the Force to influence the midichlorians to create life... ");
    touch_point_t p;
    memset(&p, 0, sizeof(p));
    while(1){
        vTaskDelay(20 / portTICK_PERIOD_MS);
        touch_get_point(&p);
        printf("x: %i, y: %i\n", p.x, p.y);

        lcd_draw_pixel(p.x, p.y, r);
    }

    printf("Restarting now.\n");
    fflush(stdout);
    esp_restart();
}
