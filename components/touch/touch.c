#include "touch.h"

//Sets up SPI device and sends init commands
void touch_init(){
    // configure CS gpio
    gpio_set_direction(TOUCH_PIN_CS, GPIO_MODE_OUTPUT);
    gpio_set_level(TOUCH_PIN_CS, 1);
    // configure SPI
    esp_err_t ret;
    spi_device_interface_config_t touch_devcfg={
        .clock_speed_hz=TOUCH_MHZ*1000*1000,
        .mode=0,
        .spics_io_num=-1, //We do this manually to chain transactions
        .queue_size=1,
    };
    ret=spi_bus_add_device(BOARD_SPI1_HOST, &touch_devcfg, &TOUCH_SPI_HANDLE);
    ESP_ERROR_CHECK(ret);
}


//Sets the Command/Data line
// void lcd_pre_transfer_callback(spi_transaction_t *t){
//     int dc=(int)t->user;
//     gpio_set_level(LCD_PIN_DC, dc);
// }
uint8_t touch_trans_cmd8(uint8_t cmd){
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    memcpy(&t.tx_data, &cmd, 2);
    t.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA;
    esp_err_t ret = spi_device_polling_transmit(TOUCH_SPI_HANDLE, &t);
    assert( ret == ESP_OK );
    return *(uint8_t*) t.rx_data;
}
uint16_t touch_trans_cmd16(uint16_t cmd){
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 16;
    memcpy(&t.tx_data, &cmd, 2);
    t.flags = SPI_TRANS_USE_RXDATA | SPI_TRANS_USE_TXDATA;
    esp_err_t ret = spi_device_polling_transmit(TOUCH_SPI_HANDLE, &t);
    assert( ret == ESP_OK );
    return *(uint16_t*) t.rx_data;
}

void touch_get_point(touch_point_t *p){
    //start transaction
    gpio_set_level(TOUCH_PIN_CS, 0);

    touch_trans_cmd8(0xB1); // Z1
    touch_trans_cmd16(0xC1); // >> 3 //Z2
    touch_trans_cmd16(0x91); // >> 3 //Y dummy

    touch_trans_cmd16(0x91); // Y1
    uint16_t y1 = (touch_trans_cmd16(0xD1) >> 3); //X1
    uint16_t x1 = (touch_trans_cmd16(0x91) >> 3); //Y2
    uint16_t y2 = (touch_trans_cmd16(0xD1) >> 3); //X2, power down
    uint16_t x2 = (touch_trans_cmd16(0x90) >> 3);
    touch_trans_cmd16(0x00);
    //end transaction
    gpio_set_level(TOUCH_PIN_CS, 1);

    uint16_t x_avg = (x1 + x2) / 2;
    uint16_t y_avg = (y1 + y2) / 2;

    p->x = (x_avg*320 / 4096);
    p->y = (y_avg*240 / 4096);


}

// touch_point_t touch_get_raw(){
//     touch_point_t p;
//     memset(&p, 0, sizeof(p));
//     touch_trans_cmd16(0xB1);                            //Read Z1
//     uint16_t z1 = touch_trans_cmd16(0xC1) >> 3;         //Read Z2
//     uint16_t z = z1 + 4095;
//
//     uint16_t z2 = touch_trans_cmd16(0x91) >> 3;         //Read X
//     z -= z2;
//     printf("TOUCH Z: %d\n", z);
//     if (z <= 400 || z == 4095) return p;
//
//     touch_trans_cmd16(0x91);                           //Read X
//     uint16_t y1 = touch_trans_cmd16(0xD1) >> 3;        //Read y
//     uint16_t x1 = touch_trans_cmd16(0x91) >> 3;        //Read x
//     uint16_t y2 = touch_trans_cmd16(0xD1) >> 3;        //Read y
//     uint16_t x2 = touch_trans_cmd16(0x91) >> 3;        //Read x
//     p.x = (x1 + x2) / 2;
//     p.y = (y1 + y2) / 2;
//     printf("TOUCH XY: %d %d\n", p.x, p.y);
//     return p;
// }
