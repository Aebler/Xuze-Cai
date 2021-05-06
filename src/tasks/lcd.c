#include "FreeRTOS.h"
#include "emp_type.h"
#include "glob_def.h"
#include "stdint.h"
#include "task.h"
#include "tasks/lcd.h"
#include "tm4c123gh6pm.h"
#include "uart0.h"
#include "shared_memory.h"
#include "semphr.h"
#include "tasks/keyboard.h"
#include <stdbool.h>

#define LF    0x0A
#define FF    0x0C
#define CR    0x0D

#define ESC   0x1B

INT8U LCD_init;
const INT8U LCD_init_sequence[] = {
    0x30, // Reset
    0x30, // Reset
    0x30, // Reset
    0x20, // Set 4bit interface
    0x28, // 2 lines Display
    0x0C, // Display ON, Cursor OFF, Blink OFF
    0x06, // Cursor Increment
    0x01, // Clear Display
    0x02, // Home
    0xFF  // stop
};

void out_LCD_low(INT8U Ch)
{
  INT8U temp;

  temp = GPIO_PORTC_DATA_R & 0x0F;
  GPIO_PORTC_DATA_R = temp | ((Ch & 0x0F) << 4);
  // GPIO_PORTD_DATA_R &= 0x7F;        // Select write
  GPIO_PORTD_DATA_R |= 0x04; // Select data mode
  GPIO_PORTD_DATA_R |= 0x08; // Set E High
  GPIO_PORTD_DATA_R &= 0xF7; // Set E Low
}

void out_LCD_high(INT8U Ch)
{
  out_LCD_low((Ch & 0xF0) >> 4);
}

void out_LCD(INT8U Ch)
{
  INT16U i;

  out_LCD_high(Ch);
  for (i = 0; i < 1000; i++)
    ;
  out_LCD_low(Ch);
}

void wr_ctrl_LCD_low(INT8U Ch) {
  INT8U temp;
  volatile int i;

  temp = GPIO_PORTC_DATA_R & 0x0F;
  temp = temp | ((Ch & 0x0F) << 4);
  GPIO_PORTC_DATA_R = temp;
  for (i = 0; i < 1000; i)
    i++;
  GPIO_PORTD_DATA_R &= 0xFB; // Select Control mode, write
  for (i = 0; i < 1000; i)
    i++;
  GPIO_PORTD_DATA_R |= 0x08; // Set E High

  for (i = 0; i < 1000; i)
    i++;

  GPIO_PORTD_DATA_R &= 0xF7; // Set E Low

  for (i = 0; i < 1000; i)
    i++;
}

void wr_ctrl_LCD_high(INT8U Ch) {
  wr_ctrl_LCD_low((Ch & 0xF0) >> 4);
}

void wr_ctrl_LCD(INT8U Ch) {
  static INT8U Mode4bit = false;
  INT16U i;

  wr_ctrl_LCD_high(Ch);
  if (Mode4bit) {
    for (i = 0; i < 1000; i++)
      ;
    wr_ctrl_LCD_low(Ch);
  } else {
    if ((Ch & 0x30) == 0x20)
      Mode4bit = TRUE;
  }
}

void lcd_task (void *params) {
  uart0_puts("lcd_task initialized\n");
  for (uint8_t i = 0; LCD_init_sequence[i] != 0xFF; i++) {
    wr_ctrl_LCD(LCD_init_sequence[i]);
    vTaskDelay(1);
  }
  uart0_puts("lcd initialized\n");
  while (1) {
    xSemaphoreTake(refresh_sem, portMAX_DELAY);
    bool stop_char_detected = false;
    for(uint8_t i=0; i<16; i++) {
      char c = lcd_image[i];
      if(c == 0 || stop_char_detected){
        stop_char_detected = true;
        c = ' ';
      }
      out_LCD(c);
      vTaskDelay(1);
    }
    for(uint8_t i=0; i<24; i++) {
      out_LCD(' ');
      vTaskDelay(1);
    }
    stop_char_detected = false;
    for(uint8_t i=0; i<16; i++) {
      char c = lcd_image[i+16];
      if(c == 0 || stop_char_detected){
        stop_char_detected = true;
        c = ' ';
      }
      out_LCD(c);
      vTaskDelay(1);
    }
    for(uint8_t i=0; i<24; i++) {
      out_LCD(' ');
      vTaskDelay(1);
    }
  };
}

