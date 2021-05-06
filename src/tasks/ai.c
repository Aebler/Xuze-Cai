#include "FreeRTOS.h"

#include "tasks/ai.h"
#include "adc.h"
#include <stdint.h>
#include "semphr.h"
#include "shared_memory.h"
#include "task.h"


void ai_task(void* param){
  init_adc();
  
  while(1){
    xSemaphoreTake(ai_value_sem, 4);
    ai_value = get_adc();
    xSemaphoreGive(ai_value_sem);
    vTaskDelay(10);
  }
}
