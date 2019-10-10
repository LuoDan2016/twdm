#include "lora.h"

//Sets up SPI device and sends init commands
void lora_init(){
    esp_err_t ret;
    spi_device_interface_config_t touch_devcfg={
        .clock_speed_hz=LORA_MHZ*1000*1000,
        .mode=0,
        .spics_io_num=LORA_PIN_CS,
        .queue_size=1,
    };
    ret=spi_bus_add_device(BOARD_SPI1_HOST, &touch_devcfg, &LORA_SPI_HANDLE);
    ESP_ERROR_CHECK(ret);
}
