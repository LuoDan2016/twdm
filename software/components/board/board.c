#include "board.h"

void board_init(){
    esp_err_t ret;
    // Initialize SPI1
    spi_bus_config_t buscfg={
        .miso_io_num=BOARD_INT_SPI_MISO,
        .mosi_io_num=BOARD_INT_SPI_MOSI,
        .sclk_io_num=BOARD_INT_SPI_CLK,
        .quadwp_io_num=-1,
        .quadhd_io_num=-1,
        .max_transfer_sz=0
    };
    ret=spi_bus_initialize(BOARD_INT_SPI_HOST, &buscfg, 1);
    ESP_ERROR_CHECK(ret);
}
