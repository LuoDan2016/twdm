#include "touch.h"

//Sets up SPI device and sends init commands
void touch_init(){
    // configure IRQ gpio
    gpio_set_direction(TOUCH_PIN_IRQ, GPIO_MODE_INPUT);
    // configure CS gpio
    // gpio_config_t io_conf;
    // io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    // io_conf.mode = GPIO_MODE_OUTPUT;
    //
    // io_conf.pin_bit_mask = (1uLL<<TOUCH_PIN_CS);
    //
    // io_conf.pull_down_en = 0;
    // io_conf.pull_up_en = 0;
    //
    // gpio_config(&io_conf);
    gpio_set_intr_type(TOUCH_PIN_CS, GPIO_INTR_DISABLE );
    gpio_set_pull_mode(TOUCH_PIN_CS, GPIO_FLOATING);
    gpio_set_direction(TOUCH_PIN_CS, GPIO_MODE_OUTPUT_OD);
    while(true){
        gpio_set_level(TOUCH_PIN_CS, 1);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        gpio_set_level(TOUCH_PIN_CS, 0);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }
    // configure SPI
    esp_err_t ret;
    spi_device_interface_config_t touch_devcfg={
        .clock_speed_hz=TOUCH_MHZ*1000*1000,
        .mode=0,
        .spics_io_num=-1, //We do this manually to chain transactions
        .queue_size=1,
    };
    ret=spi_bus_add_device(BOARD_INT_SPI_HOST, &touch_devcfg, &TOUCH_SPI_HANDLE);
    ESP_ERROR_CHECK(ret);
}

void touch_irq_init(gpio_isr_t isr_handler){
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;

    io_conf.pin_bit_mask = (1uLL<<TOUCH_PIN_IRQ);

    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;

    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(TOUCH_PIN_IRQ, isr_handler, (void*) TOUCH_PIN_IRQ);
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
