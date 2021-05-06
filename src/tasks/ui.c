#include "FreeRTOS.h"
#include "tasks/ui.h"
#include "tasks/keyboard.h"
#include "semphr.h"
#include "shared_memory.h"
#include "uart0.h"
#include "task.h"


typedef enum{
  UI_MODE_DEFAULT,
  UI_MODE_MENU,
  UI_MODE_SET_OFFSET,
  UI_MODE_SET_SCALE,
  UI_MODE_TO_MENU
} ui_mode_t;

char keys[] = {'1', '2', '3', '4', '5', '6', '7', '8', '9', '*', '0', '#'};

void ui_task(void* param){
  uart0_puts("ui_task initialized\n");
  while (1){
    static ui_mode_t ui_mode = UI_MODE_DEFAULT;
    static uint16_t temp_scalefactor = 0;
    static uint16_t temp_offset = 0;

    if (ui_mode == UI_MODE_TO_MENU) {
      // UI_MODE_TO_MENU -------------------------------------------------------- //
      ui_mode = UI_MODE_MENU;
      sprintf(lcd_image, "1: Offset %d", offset);
      sprintf(lcd_image + 16, "2: Scale %d", scalefactor);
      xSemaphoreGive(refresh_sem);
    }
    // If new messages are ready
    if(uxQueueMessagesWaiting( input_queue )){
      keyboard_button button_event;

      xSemaphoreTake(input_queue_sem, portMAX_DELAY);
      xQueueReceive(input_queue, (void*) & button_event, 10);
      xSemaphoreGive(input_queue_sem);

      int key_val = keys[button_event] - '0';


      switch (ui_mode)
      {
      // UI_MODE_DEFAULT -------------------------------------------------------- //

        case UI_MODE_DEFAULT:
          if(button_event == btn_star){
            uart0_puts("mode: menu\n");
            // Take LCD semaphore
            xSemaphoreTake(lcd_image_sem, portMAX_DELAY);
            ui_mode = UI_MODE_TO_MENU;
          }

          break;


      // UI_MODE_MENU ----------------------------------------------------------- //

        case UI_MODE_MENU:
          switch (button_event)
          {
          case btn_star:
            ui_mode = UI_MODE_DEFAULT;
            uart0_puts("mode: default\n");

            // Give LDC semaphore
            xSemaphoreGive(lcd_image_sem);
            break;

          case btn_1:
            ui_mode = UI_MODE_SET_OFFSET;
            temp_offset = 0;
            sprintf(lcd_image, "Offset");
            sprintf(lcd_image+16, "");
            xSemaphoreGive(refresh_sem);

            break;

          case btn_2:
            ui_mode = UI_MODE_SET_SCALE;
            temp_scalefactor = 0;
            sprintf(lcd_image, "Scale");
            sprintf(lcd_image+16, "");
            xSemaphoreGive(refresh_sem);

          default:
            break;
          }

          break;

      // UI_MODE_SET_OFFSET ----------------------------------------------------- //

        case UI_MODE_SET_OFFSET:
          switch (button_event)
          {
          case btn_star: // Back without save
            ui_mode = UI_MODE_TO_MENU;

            break;

          case btn_hash: // Save
            ui_mode = UI_MODE_TO_MENU;

            xSemaphoreTake(offset_sem, portMAX_DELAY);
            offset = temp_offset;
            xSemaphoreGive(offset_sem);

            break;

          default:
            temp_offset = temp_offset * 10 + key_val;

            sprintf(lcd_image, "Offset");
            sprintf(lcd_image + 16, "%d", temp_offset);
            xSemaphoreGive(refresh_sem);

            break;
          }

          break;

      // UI_MODE_SET_SCALE ------------------------------------------------------ //

        case UI_MODE_SET_SCALE:
          switch (button_event)
          {
          case btn_star: // Back without save
            ui_mode = UI_MODE_TO_MENU;

            break;

          case btn_hash: // Save
            ui_mode = UI_MODE_TO_MENU;

            xSemaphoreTake(scalefactor_sem, 0);
            scalefactor = temp_scalefactor;
            xSemaphoreGive(scalefactor_sem);

            break;

          default:
            temp_scalefactor = temp_scalefactor * 10 + key_val;

            sprintf(lcd_image, "Scale");
            sprintf(lcd_image + 16, "%d", temp_scalefactor);
            xSemaphoreGive(refresh_sem);

            break;
          }

          break;

        default:

          break;
      }

      // sprintf(msg, "scale: %d\n", scalefactor);
      // uart0_puts(msg);

      // sprintf(msg, "offset: %d\n", offset);
      // uart0_puts(msg);

      // sprintf(msg, "Mode: %d\n", ui_mode);
      // uart0_puts(msg);

    }

    // uart0_putc('0' + ui_mode);

    vTaskDelay(20);
    // taskYIELD();
  }
}
