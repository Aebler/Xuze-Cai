
#include "FreeRTOS.h"
#include "tasks/scale.h"
#include "shared_memory.h"
#include "task.h"
#include "uart0.h"

#include <stdio.h>


void scale_task(void * params){
  uart0_puts("scale task initialized\n");
  while(1){
    static uint32_t val;

    // Read potentimeter
    xSemaphoreTake(ai_value_sem, 0);
    val = ai_value;
    xSemaphoreGive(ai_value_sem);

    // Read and apply scalefactor
    xSemaphoreTake(scalefactor_sem, 0);
    val *= scalefactor;
    xSemaphoreGive(scalefactor_sem);

    // Read and apply offset
    xSemaphoreTake(offset_sem, 0);
    val += offset;
    xSemaphoreGive(offset_sem);

    // Write to LCD
    xSemaphoreTake(lcd_image_sem, portMAX_DELAY);
    sprintf(lcd_image, "Menu: *");
    sprintf(lcd_image + 16, "Value: %d", val);
    xSemaphoreGive(lcd_image_sem);
    xSemaphoreGive(refresh_sem);

    vTaskDelay(10);
  }
}
