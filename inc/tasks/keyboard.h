#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef enum {
  btn_1,
  btn_2,
  btn_3,
  btn_4,
  btn_5,
  btn_6,
  btn_7,
  btn_8,
  btn_9,
  btn_star,
  btn_0,
  btn_hash
} keyboard_button;

keyboard_button get_button_from_bit_index(uint8_t i);

void keyboard_init();

uint16_t keyboard_scan_matrix();

void keyboard_task(void* param);
