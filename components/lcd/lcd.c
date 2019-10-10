#include "lcd.h"

//Sets up SPI device and sends init commands
void lcd_init(){
    // set D/C gpio
    gpio_set_direction(LCD_PIN_DC, GPIO_MODE_OUTPUT);
    // configure device
    esp_err_t ret;
    spi_device_interface_config_t lcd_devcfg={
        .clock_speed_hz=LCD_MHZ*1000*1000,
        .mode=0,
        .spics_io_num=LCD_PIN_CS,
        .queue_size=7,
        .pre_cb=lcd_pre_transfer_callback,
    };
    ret=spi_bus_add_device(BOARD_SPI1_HOST, &lcd_devcfg, &LCD_SPI_HANDLE);
    ESP_ERROR_CHECK(ret);

    //Send init commands
    int cmd = 0;
    while (lcd_init_cmds[cmd].databytes!=0xFF) {
        lcd_cmd(lcd_init_cmds[cmd].cmd);
        lcd_data(lcd_init_cmds[cmd].data, lcd_init_cmds[cmd].databytes&0x7F);
        //If MSB set, delay for some time
        if (lcd_init_cmds[cmd].databytes&0x80) vTaskDelay(200 / portTICK_RATE_MS);
        cmd++;
    }
}

//Get pixel color
uint16_t lcd_get_color(uint8_t r, uint8_t g, uint8_t b) {
  return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | ((b & 0xF8) >> 3);
}

//Draw pixel
void lcd_draw_pixel(uint16_t x0, uint16_t y0, uint16_t color){
    if(x0 > 320 || y0 > 240) assert(0);
    lcd_setwin(x0, y0, x0+1, y0+1);
    //make a pointer of uint8, reat uint16 as if it's two uint8 sequentially
    uint8_t* ptr = (uint8_t*)&color;
    lcd_data(ptr, 2);
    lcd_cmd(0x00);
}

//Sets window
void lcd_setwin(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1){
    uint8_t col_buf[4];
    uint8_t row_buf[4];

    col_buf[0] = x0 >> 8;
    col_buf[1] = x0 & 0xFF;         // XSTART
    col_buf[2] = x1 >> 8;
    col_buf[3] = x1 & 0xFF;         // XEND

    row_buf[0] = y0 >> 8;
    row_buf[1] = y0 & 0xFF;         // YSTART
    row_buf[2] = y1 >> 8;
    row_buf[3] = y1 & 0xFF;         // YEND
    //Col data
    lcd_cmd(0x2A);
    lcd_data(col_buf, 4);      // Send 4*8bit = 2* 16bit
    //Row data
    lcd_cmd(0x2B);
    lcd_data(row_buf, 4);      // Send 4*8bit = 2* 16bit
    //Go into data transfer
    lcd_cmd(0x2C);
}

//Send len*uint8_t of data
void lcd_data(const uint8_t *data, int len){
    esp_err_t ret;
    spi_transaction_t t;
    if (len==0) return;             //no need to send anything
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=len*8;                 //Len is in bytes, transaction length is in bits.
    t.tx_buffer=data;               //Data
    t.user=(void*)1;                //D/C needs to be set to 1
    ret=spi_device_polling_transmit(LCD_SPI_HANDLE, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}

//Send uint8_t of cmd
void lcd_cmd(const uint8_t cmd){
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.length=8;                     //Command is 8 bits
    t.tx_buffer=&cmd;               //The data is the cmd itself
    t.user=(void*)0;                //D/C needs to be set to 0
    ret=spi_device_polling_transmit(LCD_SPI_HANDLE, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}

//Sets the Command/Data line
void lcd_pre_transfer_callback(spi_transaction_t *t){
    int dc=(int)t->user;
    gpio_set_level(LCD_PIN_DC, dc);
}
