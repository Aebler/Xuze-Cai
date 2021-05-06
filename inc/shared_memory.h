#pragma once

#include <stdint.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include <stdbool.h>

extern uint16_t scalefactor;
extern uint16_t offset;
extern uint16_t ai_value;
extern char lcd_image[32];

extern xQueueHandle input_queue;
extern xSemaphoreHandle scalefactor_sem, offset_sem, ai_value_sem, is_in_menu_sem, lcd_image_sem, input_queue_sem, refresh_sem;

void init_shared_memory();

