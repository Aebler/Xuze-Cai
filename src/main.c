#include "FreeRTOS.h"
#include "queue.h"
#include "systick_frt.h"
#include "task.h"
#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "shared_memory.h"
#include "uart0.h"
#include "tasks/keyboard.h"
#include "tasks/lcd.h"
#include "tasks/ui.h"
#include "tasks/scale.h"
#include "tasks/ai.h"

int putChar(char c) {
  uart0_putc(c);
  return 0;
}

void init_hardware() {
  // Enable the GPIO port that is used for the on-board LEDs and switches.
  SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF | SYSCTL_RCGC2_GPIOC | SYSCTL_RCGC2_GPIOD;
  keyboard_init();
  uart0_init(115200, 8, 1, 'n');

  // Set the direction as output (PF1 - PF3).
  GPIO_PORTF_DIR_R = 0x0E;

  // Enable the GPIO pins for digital function (PF1 - PF4)
  GPIO_PORTF_DEN_R = 0x1E;

  // Enable internal pull-up (PF4).
  GPIO_PORTF_PUR_R = 0x10;

  GPIO_PORTA_DATA_R |= 0b00011100;

  GPIO_PORTD_DIR_R = 0x4C;
  GPIO_PORTC_DIR_R = 0xF0;
  GPIO_PORTC_DEN_R = 0xF0;
  GPIO_PORTD_DEN_R = 0x4C;

  init_systick();

}

int main() {
  init_shared_memory();
  init_hardware();

  xTaskCreate(keyboard_task, "keyboard_task", 512, NULL, 1, NULL);
  xTaskCreate(scale_task, "scale_task", 512, NULL, 1, NULL);
  xTaskCreate(ui_task, "ui_task", 512, NULL, 1, NULL);
  xTaskCreate(ai_task, "ai_task", 512, NULL, 1, NULL);
  xTaskCreate(lcd_task, "myTask3", 512, NULL, 1, NULL);
  vTaskStartScheduler();
}
