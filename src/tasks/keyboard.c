#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "shared_memory.h"
#include "tasks/keyboard.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>

uint32_t key_DEF_mask = 0b00011100;
uint32_t key_GHJK_mask = 0b00001111;

void keyboard_init() {
  SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOE | SYSCTL_RCGC2_GPIOA;

  // Set the direction as output
  GPIO_PORTA_DIR_R |= key_DEF_mask;
  // Set the direction as input
  GPIO_PORTE_DIR_R &= ~key_GHJK_mask;
  // Enable the GPIO pins for digital function
  GPIO_PORTA_DEN_R |= key_DEF_mask;
  GPIO_PORTE_DEN_R |= key_GHJK_mask;
}

keyboard_button btn_from_index[] = {btn_star, btn_7, btn_4, btn_1, btn_0, btn_8, btn_5, btn_2, btn_hash, btn_9, btn_6, btn_3};
keyboard_button get_button_from_bit_index(uint8_t i) {
  i %= 12;
  return btn_from_index[i];
}

// output bits in this order: 369#2580147*
uint16_t keyboard_scan_matrix() {
  GPIO_PORTA_DATA_R &= ~key_DEF_mask;
  uint16_t res = 0;
  for (int col=0; col<3; col++) {
    GPIO_PORTA_DATA_R &= ~key_DEF_mask;
    // Power the column of the given button
    GPIO_PORTA_DATA_R |= 0b10000>>col;
    asm("nop"); // let pin rise, important
    res |= (GPIO_PORTE_DATA_R&key_GHJK_mask)<<col*4;
  }
  return res;
}

void keyboard_task(void* params) {
  uart0_puts("keyboard_task initialized\n");
  uint16_t prev_k = 0;
  while (1) {
    uint16_t k = keyboard_scan_matrix();
    uint16_t kk = (k^prev_k) & k;
    if (kk!=0) {
      for(int i = 0; i<=btn_hash; i++) {
        if (kk & (1<<i)) {
          keyboard_button btn = get_button_from_bit_index(i);
          xSemaphoreTake(input_queue_sem, 10);
          xQueueSendToFront(input_queue, (void*)&btn, 10);
          xSemaphoreGive(input_queue_sem);
        }
      }
    }
    prev_k = k;

    vTaskDelay(20/5);
  }
}
