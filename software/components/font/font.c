#include "font.h"

void lcd_draw_char(uint16_t x0, uint16_t y0, char ascii){
    lcd_setwin(x0, y0, x0+15,y0+15);
    // Each color is two bytes, thus 16*16 colors * 2 = 512 uint8 bytes
    uint8_t *dma_buffer = heap_caps_malloc(512, MALLOC_CAP_DMA);
    uint16_t bg = lcd_get_color(0x00, 0x00, 0x00);
    uint16_t color = lcd_get_color(0xFF, 0xFF, 0xFF);
    // Remove ASCII control codes
    // If wraparound or out of bounds
    uint16_t char_offset = ascii-32;
    if(char_offset > 94) return;
    char_offset *= 32;
    uint16_t dma_buf_idx = 0;
    //Fill buffer
    for(uint16_t char_ptr = char_offset; char_ptr < char_offset+32; char_ptr+=2){
        uint8_t l = font_16[char_ptr];
        uint8_t r = font_16[char_ptr+1];
        for(int i = 0; i < 8; i++){
            if(l<<i & 0x80){
                dma_buffer[dma_buf_idx] = color >> 8;
                dma_buffer[dma_buf_idx+1] = color & 0xFF;
            }
            else{
                dma_buffer[dma_buf_idx] = bg >> 8;
                dma_buffer[dma_buf_idx+1] = bg & 0xFF;
            }
            dma_buf_idx+=2;
        }
        for(int i = 0; i < 8; i++){
            if(r<<i & 0x80){
                dma_buffer[dma_buf_idx] = color >> 8;
                dma_buffer[dma_buf_idx+1] = color & 0xFF;
            }
            else{
                dma_buffer[dma_buf_idx] = bg >> 8;
                dma_buffer[dma_buf_idx+1] = bg & 0xFF;
            }
            dma_buf_idx+=2;
        }
    }
    lcd_data(dma_buffer, 512);
    heap_caps_free(dma_buffer);
}

void lcd_draw_str(uint16_t x0, uint16_t y0, char* str){
    uint16_t cx = x0;
    uint16_t cy = y0;
    int i = 0;
    while(str[i] != '\0'){
        lcd_draw_char(cx, cy, str[i]);
        //Go to next char
        i++;
        //Increment x with 16 for next pos
        cx += 16;
        //If over x width, reset to start x and inc y
        if(cx+16 > 320){
            cx = x0;
            cy += 16;
        }
        //Outside of window
        if(cy+16 > 240) return;

    }
}
