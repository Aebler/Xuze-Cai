#include "shared_memory.h"

#define INPUT_QUEUE_SIZE 10


uint16_t scalefactor = 1;
uint16_t offset = 0;
uint16_t ai_value = 0;
char lcd_image[32] = {};

xQueueHandle input_queue;
xSemaphoreHandle scalefactor_sem, offset_sem, ai_value_sem, is_in_menu_sem, lcd_image_sem, input_queue_sem, refresh_sem;


void init_shared_memory(){
  /* for(int i = 0; i < 32; i++){ */
  /*   lcd_image[i] = 0; */
  /* } */

  input_queue = xQueueCreate(INPUT_QUEUE_SIZE, sizeof(char));

  input_queue_sem = xSemaphoreCreateMutex();
  scalefactor_sem = xSemaphoreCreateMutex();
  offset_sem = xSemaphoreCreateMutex();
  ai_value_sem = xSemaphoreCreateMutex();
  is_in_menu_sem = xSemaphoreCreateMutex();
  lcd_image_sem = xSemaphoreCreateMutex();
  refresh_sem = xSemaphoreCreateMutex();

}

